/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:41:01 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/16 10:22:15 by komatsud         ###   ########.fr       */
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

bool AMethod::checkMaxBodySize(unsigned long long __size) const {
	// LocationのMaxBodySizeをチェック
	if (isloc == true && __size > loc.getMaxBodySize()) {
		return (false);
	}
	// ConfigのMaxBodySizeをチェック
	if (__size > conf.getMaxBodySize()) {
		return (false);
	}
	return (true);
}

Result<std::string, bool> const AMethod::_openFile(std::string filename) {
	// open
	std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
	if (!ifs && errno == ENOENT) {
#if defined(_DEBUGFLAG)
		std::cout << RED << "AMethod::_openFile open失敗。ENOENT" << RESET
				  << std::endl;
		std::cout << RED << "Filename: " << filename << RESET << std::endl;
#endif
		res.setStatus(404);
		res.setStatusMessage(statusmap.at(404));
		return Error<bool>(false);
	} else if (!ifs && errno == EACCES) {
		res.setStatus(403);
		res.setStatusMessage(statusmap.at(403));
		return Error<bool>(false);
	} else if (!ifs) {
#if defined(_DEBUGFLAG)
		std::cout << RED
				  << "AMethod::_openFile "
					 "open失敗。エラーコードがHTTPステータスコードと対応しない"
				  << RESET << std::endl;
		std::cout << RED << "Filename: " << filename << RESET << std::endl;
#endif
		res.setStatus(500);
		res.setStatusMessage(statusmap.at(500));
		res.setHeader("Connection", "close");
		return Error<bool>(false);
	}

	// サイズを測る
	ifs.seekg(0, std::ios::end);
	long long int size = ifs.tellg();
	ifs.seekg(0);

	// read
	std::string body;
	char buf[size + 1];

	ifs.read(buf, size);

	if ((ifs.rdstate() & std::ios_base::failbit) != 0 ||
		(ifs.rdstate() & std::ios_base::badbit) != 0) {
#if defined(_DEBUGFLAG)
		std::cout << RED << "AMethod::_openFile read失敗。" << RESET
				  << std::endl;
		std::cout << RED << "Filename: " << filename << RESET << std::endl;
#endif
		res.setStatus(500);
		res.setStatusMessage(statusmap.at(500));
		res.setHeader("Connection", "close");
		return Error<bool>(false);
	}

	buf[size] = '\0';
	body.assign(buf, size);

	ifs.close();

	// Bodyの読み込みが成功していたら、bodysizeとBodyをセットして返る
	std::stringstream ss;
	std::string length;
	ss << size;
	ss >> length;
	res.setHeader("Content-Length", length);
	res.setBody(body);
	return Ok<std::string>(body);
}

void AMethod::setErrorPageBody() {
	unsigned int const orist = res.getStatus();
	std::string const oristm = res.getStatusMessage();

	// Locationの指定を優先して参照する
	if (isloc == true) {
		Result<std::string, bool> locres = loc.getErrorPages(res.getStatus());

		// Locationの中にエラーページの設定が存在したらまずそちらを確認する
		if (locres.isOK() == true) {
			std::string fln = locres.getOk();
			Result<std::string, bool> locres_2 = _openFile(fln);
			if (locres_2.isOK() == true) {
				return;
			} else {
				res.setStatus(orist);
				res.setStatusMessage(oristm);
				res.setHeader("Content-Length", "0");
				return;
			}
			// Locationの設定が存在しなかった場合などにConfigの中身を確認する
		}
	}

	//次にConfigのエラーページの設定を確認する
	Result<std::string, bool> res_1 = conf.getErrorPages(res.getStatus());

	//エラーページの設定が存在しなかったとき
	if (res_1.isOK() == false) {
		res.setStatus(orist);
		res.setStatusMessage(oristm);
		std::stringstream ss;
		ss << "<!DOCTYPE html>\n";
		ss << "<html>\n";
		ss << "<head>\n";
		ss << "</head>\n";
		ss << "<body>\n";
		ss << "<h1>Error ";
		ss << orist;
		ss << " ";
		ss << oristm;
		ss << "</h1>\n";
		ss << "<p>&nbsp;</p>\n";
		ss << "<p style=\"text-align: center;\">webserv</p>";
		ss << "</body>\n";
		ss << "</html>\n";
		std::string __body;
		__body = ss.str();
		// std::cerr << RED << "__body: " << __body << RESET << std::endl;
		res.setBody(__body);
		unsigned long long __bodysize;
		std::string _bodysize;
		__bodysize = __body.size();
		std::stringstream __sb;
		__sb << __bodysize;
		__sb >> _bodysize;
		// std::cerr << RED << "bodysize: " << _bodysize << RESET << std::endl;
		res.setHeader("Content-Length", _bodysize);
		return;
	}

	//エラーページのファイル名をとってくる
	std::string filename = res_1.getOk();

	// bodyをセットする
	// bodyのセット(openとか・・・)に失敗した場合は、Bodyなしでヘッダだけ送付する
	Result<std::string, bool> res_2 = _openFile(filename);
	if (res_2.isOK() == true) return;
	//エラーページがなければ、Content-Lengthを0にセットして終了
	else {
		res.setStatus(orist);
		res.setStatusMessage(oristm);
		res.setHeader("Content-Length", "0");
		return;
	}
}

Result<int, bool> AMethod::checkURI() {
	std::string origin = req.getUrl();
	std::string rel;

	// std::cout << RED << "origin: " << origin << RESET << std::endl;

	//ここRFC見てもっと判定頑張らないとならない部分です
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
		// リクエストのURIが/から始まっていなかった時にBad Requestとして処理する
#if defined(_DEBUGFLAG)
		std::cout << RED << "URIが/から始まってないよ〜(；ω；)" << RESET
				  << std::endl;
#endif
		res.setStatus(400);
		res.setStatusMessage(statusmap.at(400));
		return Error<bool>(false);
	}

	std::string tmp;
	std::stringstream sb;

	// クエリを一旦無視する
	if (rel.find("?") != std::string::npos) {
		sb << origin;
		std::getline(sb, tmp, '?');
		rel = tmp;
		std::getline(sb, tmp, '?');
		query = tmp;
	}

	// // サーバーのルートより上を見ようとしていないかチェック
	// いりませんでした。ルートより上を見ようとしていた場合はルートのパスに変更されて届くみたい
	// std::stringstream	sc;
	// std::string			tmp;
	// int					counter;
	// std::cout << "rel: " << rel << std::endl;
	// sc << rel;

	// std::vector<std::string> t = lineSpliter(conf.getRootDir(), "/");
	// counter = t.size() - 1;
	// std::cout << "length of root: " << counter << std::endl;

	// while (sc.eof() == false) {
	// 	std::getline(sc, tmp, '/');
	// 	std::cout << "tmp: " << tmp << std::endl;
	// 	if (tmp.empty() == true || tmp == ".")
	// 	{

	// 	}
	// 	else if (tmp == "..")
	// 	{
	// 		counter --;
	// 	}
	// 	else
	// 	{
	// 		counter ++;
	// 	}
	// }
	// if (counter < 0)
	// {
	// 	#if defined(_DEBUGFLAG)
	// 		std::cout << RED << "AMethod::setUri: URI tried forbidden access" <<
	// RESET << std::endl; 	#endif 	res.setStatus(400);
	// 	res.setStatusMessage(statusmap.at(400));
	// 	setErrorPageBody();
	// }

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
	std::string locpath;
	ss << uri;
	isloc = false;
	std::getline(ss, tmp, '/');
	while (ss.eof() == false) {
		shortpath += tmp;
		shortpath += '/';
		// std::cout << shortpath << std::endl;

		// locationの設定が適用されるか否か、されるとしたらどのLocationかを検索する
		if (conf.getLocations(shortpath).isOK() == true) {
			isloc = true;
			loc = conf.getLocations(shortpath).getOk();
			locpath = shortpath;
		}
		std::getline(ss, tmp, '/');
	}

	// Locationの設定があった場合、
	//  1.
	//  aliasがあったらaliasを優先で適用する(LocationのRootの部分を置き換える)
	//  2. Locationの中のRootを見て、それがあれば頭にただくっつける
	if (isloc == true) {
		// aliasを適用(Locationで指定されているパスをAliasの指定で置き換える)
		if (loc.getAlias().empty() == false) {
			std::string alpath = loc.getAlias() + '/';
			while (1) {
				size_t pos = uri.find(locpath);
				if (pos == std::string::npos) break;
				size_t len = locpath.length();
				uri.replace(pos, len, alpath);
			}
		}
		// locationのrootをくっつける(URIの頭につけるだけ)
		if (loc.getRootDir().empty() == false) {
			tmp = loc.getRootDir() + '/' + uri;
			uri = tmp;
		}
	}

	// std::cout << "uri: " << uri << std::endl;

	// uri_without_rootを設定、スラッシュも二重のやつをなくす
	// これは必ずTrailing slash ありにする
	uri_without_root = uri;
	while (1) {
		size_t pos = uri_without_root.find("//");
		if (pos == std::string::npos) break;
		size_t len = 2;
		uri_without_root.replace(pos, len, "/");
	}
	if (uri_without_root.find_last_of('/') != uri_without_root.size() - 1) {
		uri_without_root += '/';
	}

	// ConfigのrootがあればさらにURIの頭にくっつける
	if (conf.getRootDir().empty() == false) {
		tmp = conf.getRootDir() + '/' + uri;
		uri = tmp;
	}

	//二重のスラッシュがあったら一つにする
	while (1) {
		size_t pos = uri.find("//");
		if (pos == std::string::npos) break;
		size_t len = 2;
		uri.replace(pos, len, "/");
	}

	// cgiかどうかチェック！！
	iscgi = false;
	iscgicanaccess = false;
	std::stringstream sb;
	std::string cgipath;
	sb << uri;

	// URIのパスを上からどんどんくっつけていき、Cgiの拡張子を探す
	while (sb.eof() == false) {
		std::getline(sb, tmp, '/');
		cgipath += tmp;
		// std::cout << "cgipath: " << cgipath << std::endl;
		if (cgipath.find('.') != std::string::npos) {
			std::vector<std::string> lines;
			lines = lineSpliter(tmp, ".");
			if (lines.size() == 2) {
				// locationのcgiExtensionの指定を見る
				if (isloc == true) {
					Result<int, bool> _res = loc.getCgiExtension(lines.at(1));
					if (_res.isOK() == true) {
						//これはcgiだ！
						iscgi = true;
						path_to_cgi = cgipath;
						break;
					}
				}
				// ConfigのcgiExtensionの指定を見る
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

	//最初に.をつけて開けるようにする(要審議...)
	tmp = "." + uri;
	uri = tmp;
	// cgi本体(拡張子を含むところまでのパス)を指したパスにも頭に.をつけて開けるようにする
	tmp = "." + cgipath;
	path_to_cgi = tmp;

	/* 例えば、/content/test.py/test?test=query ときたら   */
	/* uri = "./content/test.py/test?test=query"		*/
	/* path_to_cgi = "./content/test.py" 				*/

	// cgiだった場合のファイルの権限・存在チェックをする
	if (iscgi == true) {
		int status;
		errno = 0;

		// std::cout << "uri: " << uri << std::endl;
		// std::cout << "path_to_cgi: " << path_to_cgi << std::endl;

		status = access(path_to_cgi.c_str(), X_OK);
		if (status == -1) {
			iscgicanaccess = false;
			if (errno == EACCES) {
#if defined(_DEBUGFLAG)
				std::cout << RED << "AMethod::setUri: CGI Path Check...EACCES"
						  << RESET << std::endl;
#endif
				res.setStatus(403);
				res.setStatusMessage(statusmap.at(403));
				setErrorPageBody();
			} else if (errno == ENOENT) {
#if defined(_DEBUGFLAG)
				std::cout << RED << "AMethod::setUri: CGI Path Check...ENOENT"
						  << RESET << std::endl;
#endif
				res.setStatus(404);
				res.setStatusMessage(statusmap.at(404));
				setErrorPageBody();
			} else if (errno == ENAMETOOLONG) {
#if defined(_DEBUGFLAG)
				std::cout << RED
						  << "AMethod::setUri: sCGI Path Check...ENAMETOOLONG"
						  << RESET << std::endl;
#endif
				res.setStatus(414);
				res.setStatusMessage(statusmap.at(414));
				setErrorPageBody();
			} else if (errno == ELOOP) {
#if defined(_DEBUGFLAG)
				std::cout << RED << "AMethod::setUri: sCGI Path Check...ELOOP"
						  << RESET << std::endl;
#endif
				res.setStatus(500);
				res.setStatusMessage(statusmap.at(500));
				res.setHeader("Connection", "close");
				setErrorPageBody();
			} else {
#if defined(_DEBUGFLAG)
				std::cout << RED
						  << "AMethod::setUri: sCGI Path Check...Some error "
							 "with access()"
						  << RESET << std::endl;
#endif
				res.setStatus(400);
				res.setStatusMessage(statusmap.at(400));
				setErrorPageBody();
			}
		} else {
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
		} else if (loc.getReturnUrl().empty() == false) {
			res.addHeader("Location", loc.getReturnUrl());
		} else {
			setErrorPageBody();
		}
		return Ok<int>(0);
	}
	// config指定のリダイレクト
	if (conf.isReturn() == true) {
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
		} else if (conf.getReturnUrl().empty() == false) {
			res.addHeader("Location", conf.getReturnUrl());
		} else {
			setErrorPageBody();
		}
		return Ok<int>(0);
	}

	// rewriteの確認
	if (isloc == true) {
		Result<std::string, bool> resr_loc = loc.getRedirects(req.getUrl());
		if (resr_loc.isOK() == true) {
			res.setStatus(302);
			res.setStatusMessage(statusmap.at(302));
			res.addHeader("Location", resr_loc.getOk());
			return Ok<int>(0);
		}
	}
	Result<std::string, bool> resr_conf = conf.getRedirects(req.getUrl());
	if (resr_conf.isOK() == true) {
		res.setStatus(302);
		res.setStatusMessage(statusmap.at(302));
		res.addHeader("Location", resr_conf.getOk());
		return Ok<int>(0);
	}

	return Error<bool>(true);
}

Result<std::string, bool> const AMethod::isCgi() const {
	// std::cout << "path to cgi: " << path_to_cgi << std::endl;

	// CGIが指定されていて、しかもそのファイルが実際にアクセス可能な時のみOkを返す
	// Okの中にCGIへのパスが詰まっている
	if (iscgi == true && iscgicanaccess == true) {
		return Ok<std::string>(path_to_cgi);
	}

	// CGIが指定されていたが、そのファイルがアクセスできなかった場合、
	// ErrorのTrueを返す(ひどい設計だ・・・ごめんなさい・・・)
	else if (iscgi == true) {
		return Error<bool>(true);
	}

	// CGiが指定されていなかった場合、ErrorでFalseを返します
	return Error<bool>(false);
}

std::string const &AMethod::getQuery() const { return (query); }
