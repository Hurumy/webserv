/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:41:01 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 12:11:32 by komatsud         ###   ########.fr       */
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
	m[303] = "See Other";
	m[304] = "Not Modified";
	m[305] = "Use Proxy";
	m[306] = "(Unused)";
	m[307] = "Temporary Redirect";
	m[308] = "Permanent Redirect";
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
	m[418] = "(Unused)";
	m[421] = "Misdirected Request";
	m[422] = "Unprocessable Content";
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
		res.setStatusMessage(statusmap.at(404));
		return Error<bool>(false);
	} else if (fd == -1 && errno == EACCES) {
		res.setStatus(403);
		res.setStatusMessage(statusmap.at(403));
		return Error<bool>(false);
	} else if (fd == -1)
	{
		res.setStatus(500);
		res.setStatusMessage(statusmap.at(500));
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
		res.setStatusMessage(statusmap.at(500));
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
	Result<std::string, bool> res_1 = conf.getErrorPages(res.getStatus());

	//エラーページの設定が存在しなかったとき
	if (res_1.isOK() == false)
	{
		res.addHeader("Content-Length", "0");
		return;
	}

	//エラーページのファイル名をとってくる
	std::string filename = res_1.getOk();

	// bodyをセットする。成功したら抜けるループ
	// bodyのセット(openとか・・・)に失敗した場合は、Bodyなしでヘッダだけ送付する
	while (1)
	{
		Result<std::string, bool> res_2 = _openFile(filename);
		if (res_2.isOK() == true)
			break;
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
		res.setStatusMessage(statusmap.at(400));
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
	std::string	locpath;
	ss << uri;
	isloc = false;
	std::getline(ss, tmp, '/');
	while (ss.eof() == false)
	{
		shortpath += tmp;
		shortpath += '/';
		//std::cout << shortpath << std::endl;

		//locationの設定が適用されるか否か、されるとしたらどのLocationかを検索する
		if (conf.getLocations(shortpath).isOK() == true)
		{
			isloc = true;
			loc = conf.getLocations(shortpath).getOk();
			locpath = shortpath;
		}
		std::getline(ss, tmp, '/');
	}

	//Locationの設定があった場合、
	// 1. aliasがあったらaliasを優先で適用する(LocationのRootの部分を置き換える)
	// 2. Locationの中のRootを見て、それがあれば頭にただくっつける
	if (isloc == true)
	{
		//aliasを適用(Locationで指定されているパスをAliasの指定で置き換える)
		if (loc.getAlias().empty() == false)
		{
			std::string alpath = loc.getAlias() + '/';
			while (1)
			{
				size_t	pos = uri.find(locpath);
				if (pos == std::string::npos)
					break;
				size_t	len = locpath.length();
				uri.replace(pos, len, alpath);
			}
		}
		//locationのrootをくっつける(URIの頭につけるだけ)
		if (loc.getRootDir().empty() == false)
		{
			tmp = loc.getRootDir() + '/' + uri;
			uri = tmp;
		}
	}

	//std::cout << "uri: " << uri << std::endl;

	// ConfigのrootがあればさらにURIの頭にくっつける
	if (conf.getRootDir().empty() == false) {
		tmp = conf.getRootDir() + '/' + uri;
		uri = tmp;
	}

	// cgiかどうかチェック！！
	iscgi = false;
	iscgicanaccess = false;
	std::stringstream sb;
	std::string cgipath;
	sb << uri;

	//URIのパスを上からどんどんくっつけていき、Cgiの拡張子を探す
	while (sb.eof() == false) {
		std::getline(sb, tmp, '/');
		cgipath += tmp;
		if (tmp.find('.') != std::string::npos) {
			std::vector<std::string> lines;
			lines = lineSpliter(tmp, ".");
			if (lines.size() == 2) {
				// locationのcgiExtensionの指定を見る
				if (isloc == true) {
					Result<int, bool> _res = loc.getCgiExtension(lines.at(1));
					if (_res.isOK() == true)
					{
						//これはcgiだ！
						iscgi = true;
						path_to_cgi = cgipath;
						break;
					}
				}
				// ConfigのcgiExtensionの指定を見る
				else {
					Result<int, bool> _res = conf.getCgiExtension(lines.at(1));
					if (_res.isOK() == true)
					{
						//これはcgiだ！
						iscgi = true;
						break;
					}
				}
			}
		}
		cgipath += "/";
	}

	//二重のスラッシュがあったら一つにする
	while (1)
	{
		size_t pos = uri.find("//");
		if (pos == std::string::npos)
			break;
		size_t len = 2;
		uri.replace(pos, len, "/");
	}

	//最初に.をつけて開けるようにする(要審議...)
	tmp = "." + uri;
	uri = tmp;

	path_to_cgi = uri;

	//cgiだった場合のファイルの権限・存在チェックをする
	if (iscgi == true)
	{
		int	status;
		errno = 0;

		//std::cout << "uri: " << uri << std::endl;

		status = access(uri.c_str(), X_OK);
		if (status == -1)
		{
			iscgicanaccess = false;
			if (errno == EACCES)
			{
				res.setStatus(403);
				res.setStatusMessage(statusmap.at(403));
				setErrorPageBody();
			}
			else if (errno == ENOENT)
			{
				res.setStatus(404);
				res.setStatusMessage(statusmap.at(404));
				setErrorPageBody();
			}
			else if (errno == ENAMETOOLONG)
			{
				res.setStatus(414);
				res.setStatusMessage(statusmap.at(414));
				setErrorPageBody();
			}
			else if (errno == ELOOP)
			{
				res.setStatus(500);
				res.setStatusMessage(statusmap.at(500));
				setErrorPageBody();
			}
			else
			{
				res.setStatus(400);
				res.setStatusMessage(statusmap.at(400));
				setErrorPageBody();
			}
		}
		else
		{
			iscgicanaccess = true;
		}
	}



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

Result<std::string, bool> const AMethod::isCgi() const
{

	//std::cout << "path to cgi: " << path_to_cgi << std::endl;

	//CGIが指定されていて、しかもそのファイルが実際にアクセス可能な時のみOkを返す
	//Okの中にCGIへのパスが詰まっている
	if (iscgi == true && iscgicanaccess == true)
	{
		return Ok<std::string>(path_to_cgi);
	}
	
	//CGIが指定されていたが、そのファイルがアクセスできなかった場合、
	//ErrorのTrueを返す(ひどい設計だ・・・ごめんなさい・・・)
	else if (iscgi == true)
	{
		return Error<bool>(true);
	}

	//CGiが指定されていなかった場合、ErrorでFalseを返します
	return Error<bool>(false);
}
