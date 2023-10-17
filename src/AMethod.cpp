/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:41:01 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/17 14:34:05 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

const std::map<unsigned int, std::string> AMethod::statusmap =
	AMethod::initStatusMap();

std::map<unsigned int, std::string> AMethod::initStatusMap() {
	std::map<unsigned int, std::string> m;
	m[100] = "Continue";
	m[101] = "Switching Protocol";
	m[102] = "Processing";
	m[103] = "Early Hints";
	m[200] = "OK";
	m[201] = "Created";
	m[202] = "Accepted";
	m[203] = "Non-Authoritative Information";
	m[204] = "No Content";
	m[205] = "Reset Content";
	m[206] = "Partial Content";
	m[300] = "Multiple Choice";
	m[301] = "Moved Permanently";
	m[302] = "Found";
	m[304] = "Not Modified";
	m[400] = "Bad Request";
	m[401] = "Unauthorized";
	m[402] = "Payment Required";
	m[403] = "Forbidden";
	m[404] = "Not Found";
	m[405] = "Method Not Allowed";
	m[406] = "Not Acceptable";
	m[407] = "Proxy Authentication Required";
	m[408] = "Request Timeout";
	m[409] = "Conflict";
	m[410] = "Gone";
	m[411] = "Length Required";
	m[412] = "Precondition Failed";
	m[413] = "Payload Too Large";
	m[414] = "URI Too Long";
	m[415] = "Unsupported Media Type";
	m[416] = "Range Not Satisfiable";
	m[417] = "Expectation Failed";
	m[422] = "Unprocessable Entity";
	m[423] = "Locked";
	m[425] = "Too Early";
	m[426] = "Upgrade Required";
	m[429] = "Too Many Requests";
	m[431] = "Request Header Fields Too Large";
	m[500] = "Internal Server Error";
	m[501] = "Not Implemented";
	m[502] = "Bad Gateway";
	m[503] = "Service Unavailable";
	m[504] = "Gateway Timeout";
	m[505] = "HTTP Version Not Supported";
	m[506] = "Variant Also Negotiates";
	m[507] = "Insufficient Storage";
	m[508] = "Loop Detected";
	m[510] = "Not Extended";
	m[511] = "Network Authentication Required";
	return m;
}

AMethod::AMethod(Config _conf, Request _req, Response &_res)
	: conf(_conf), req(_req), res(_res) {}

AMethod::~AMethod() {}

Result<std::string, bool> const AMethod::_openFile(std::string filename) {
	int fd;
	unsigned long long bodysize = 0;
	int status = 1;
	std::string body;
	char buf[FILE_READ_SIZE];

	// open
	fd = open(filename.c_str(), O_RDONLY);
	if (fd == -1 && errno == ENOENT) {
		res.setStatus(404);
		res.setStatusMessage("Not Found");
		return Error<bool>(false);
	} else if (fd == -1 && errno == EACCES) {
		res.setStatus(403);
		res.setStatusMessage("Forbidden");
		return Error<bool>(false);
	} else if (fd == -1)
	{
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return Error<bool>(false);
	}
	
	// read
	while (status > 0) {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status != -1) {
			buf[status] = '\0';
			body += buf;
			bodysize += status;
		}
	}
	close(fd);

	if (status == -1) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return Error<bool>(false);
	}

	// Bodyの読み込みが成功していたら、bodysizeとBodyをセットして返る
	std::stringstream ss;
	std::string length;
	ss << bodysize;
	ss >> length;
	res.addHeader("Content-Length", length);
	res.setBody(body);
	return Ok<std::string>(body);
}

void AMethod::setErrorPageBody()
{
	unsigned int prevstatus = res.getStatus();
	Result<std::string, bool> res_1 = conf.getErrorPages(res.getStatus());

	if (res_1.isOK() == false) return;

	std::string filename = res_1.getOk();

	// bodyをセットする。成功したら抜けるループ
	// bodyのセットに失敗した場合は、失敗した後のステータスが変わっていればもう一度ファイルを検索する
	// 変わっていなければBodyなしでヘッダだけ送付する
	while (1)
	{
		Result<std::string, bool> res_2 = _openFile(filename);
		if (res_2.isOK() == true)
			break;
		//今のエラー番号にあったエラーページのファイル名を取得する
		else if (res_2.isOK() == false && prevstatus != res.getStatus())
		{
			prevstatus = res.getStatus();
			Result<std::string, bool> res_3 = conf.getErrorPages(res.getStatus());
			if (res_3.isOK() == false)
			{
				res.addHeader("Content-Length", "0");
				break;
			}
			filename = res_3.getOk();
		}
		//エラーページがなければ、Content-Lengthを0にセットして終了
		else
		{
			res.addHeader("Content-Length", "0");
			break;
		}
	}
	return;
}

Result<int, bool> AMethod::checkURI() {
	std::string origin = req.getUrl();
	std::string rel;

	//絶対URIか相対URIか判定する
	if (origin.find("http://") == 0 || origin.find("https://") == 0) {
		std::string tmp;
		std::stringstream ss;
		size_t i = 0;

		ss << origin;
		while (std::getline(ss, tmp, '/')) {
			if (i > 2 && tmp.empty() == false) {
				rel += '/';
				rel += tmp;
			}
			i++;
		}
		// std::cout << rel << std::endl;

	} else if (origin.find("/") == 0) {
		rel = req.getUrl();
	} else {
		res.setStatus(400);
		res.setStatusMessage("Bad Request");
		return Error<bool>(false);
	}

	//　サーバー自体のルートより上を見ようとしていないか、
	//　変な指定がないかなどのチェックを足す

	uri = rel;
	return Ok<int>(0);
}

void AMethod::setURI() {
	std::string tmp;

	// std::cout << uri << std::endl;

	// uriを一つずつ長くしていって、最長一致なLocationを探す
	std::stringstream ss;
	std::string shortpath;
	ss << uri;
	isloc = false;
	std::getline(ss, tmp, '/');
	while (ss.eof() == false) {
		shortpath += tmp;
		shortpath += '/';
		// std::cout << RED << shortpath << RESET << std::endl;
		if (conf.getLocations(shortpath).isOK() == true) {
			isloc = true;
			loc = conf.getLocations(shortpath).getOk();
		}
		std::getline(ss, tmp, '/');
	}

	// ROOTをURIの頭にくっつける
	if (conf.getRootDir().empty() == false) {
		tmp = conf.getRootDir() + uri;
		uri = tmp;
	}

	// cgiかどうかチェック！！
	iscgi = false;
	std::stringstream sb;
	std::string cgipath;
	sb << uri;
	while (sb.eof() == false) {
		std::getline(sb, tmp, '/');
		cgipath += tmp;
		if (tmp.find('.') != std::string::npos) {
			std::vector<std::string> lines;
			lines = lineSpliter(tmp, ".");
			if (lines.size() == 2) {
				// locationの指定を見る
				if (isloc == true) {
					Result<int, bool> _res = loc.getCgiExtension(lines.at(1));
					if (_res.isOK() == true) {
						//これはcgiだ！
						iscgi = true;
						path_to_cgi = cgipath;
						break;
					}
				}
				// Configの指定を見る
				else {
					Result<int, bool> _res = conf.getCgiExtension(lines.at(1));
					if (_res.isOK() == true) {
						//これはcgiだ！
						iscgi = true;
						path_to_cgi = cgipath;
						break;
					}
				}
			}
		}
		cgipath += "/";
	}

	//最初に.をつけて開けるようにする
	tmp = "." + uri;
	uri = tmp;

	// std::cout << YELLOW << uri << RESET << std::endl;

	return;
}

Result<int, bool> AMethod::checkRedirects() {
	//リダイレクトなどを確認する
	// location指定のリダイレクト
	std::stringstream ss;
	std::string size;

	if (isloc == true && loc.isReturn() == true) {
		res.setStatus(loc.getReturnStatus());
		if (statusmap.find(loc.getReturnStatus()) != statusmap.end())
			res.setStatusMessage(statusmap.at(loc.getReturnStatus()));
		else
			res.setStatusMessage("Unknown status");
		if (loc.getReturnBody().empty() == false) {
			ss << loc.getReturnBody().size();
			ss >> size;
			res.addHeader("Content-Length", size);
			res.addHeader("Content-Type", "text/html");
			res.setBody(loc.getReturnBody());
		}
		if (loc.getReturnUrl().empty() == false) {
			res.addHeader("Location", loc.getReturnUrl());
		}
		return Ok<int>(0);
	}
	// config指定のリダイレクト
	else if (conf.isReturn() == true) {
		res.setStatus(conf.getReturnStatus());
		if (statusmap.find(conf.getReturnStatus()) != statusmap.end())
			res.setStatusMessage(statusmap.at(conf.getReturnStatus()));
		else
			res.setStatusMessage("Unknown status");
		if (conf.getReturnBody().empty() == false) {
			ss << conf.getReturnBody().size();
			ss >> size;
			res.addHeader("Content-Length", size);
			res.addHeader("Content-Type", "text/html");
			res.setBody(conf.getReturnBody());
		} else {
			res.addHeader("Content-Length", "0");
		}
		if (conf.getReturnUrl().empty() == false) {
			res.addHeader("Location", conf.getReturnUrl());
		}
		return Ok<int>(0);
	}
	return Error<bool>(true);
}

Result<std::string, bool> const AMethod::isCgi() const {
	if (iscgi == true) {
		return Ok<std::string>(path_to_cgi);
	}
	return Error<bool>(false);
}
