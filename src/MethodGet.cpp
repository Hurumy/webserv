/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:09:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/15 10:23:05 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodGet.hpp"

//条件付きGet
// If-Modified-Since
// If-Unmodified-Since,
// If-Match
// If-None-Match
// If-Range

//部分Get
// Range

const std::map<std::string, std::string> MethodGet::ext = initExtMap();

std::map<std::string, std::string> MethodGet::initExtMap() {
	std::map<std::string, std::string> tmp;
	tmp["html"] = "text/html";
	tmp["csv"] = "text/csv";
	tmp["css"] = "text/css";
	tmp["js"] = "text/javascript";
	tmp["json"] = "application/json";
	tmp["pdf"] = "application/pdf";
	tmp["xls"] = "application/vnd.ms-excel";
	tmp["xlsx"] =
		"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	tmp["ppt"] = "application/vnd.ms-powerpoint";
	tmp["pptx"] =
		"application/"
		"vnd.openxmlformats-officedocument.presentationml.presentation";
	tmp["doc"] = "application/msword";
	tmp["docx"] =
		"application/"
		"vnd.openxmlformats-officedocument.wordprocessingml.document";
	tmp["jpg"] = "image/jpeg";
	tmp["jpeg"] = "image/jpeg";
	tmp["png"] = "image/png";
	tmp["gif"] = "image/gif";
	tmp["bmp"] = "image/bmp";
	tmp["dib"] = "image/bmp";
	tmp["svg"] = "image/svg+xml";
	tmp["zip"] = "application/zip";
	tmp["lzh"] = "application/x-lzh";
	tmp["tar"] = "application/x-tar";
	tmp["gz"] = "application/x-tar";
	tmp["mp3"] = "audio/mpeg";
	tmp["mp4"] = "video/mp4";
	tmp["mpeg"] = "video/mpeg";
	tmp["exe"] = "application/octet-stream";
	tmp["out"] = "application/octet-stream";
	tmp["txt"] = "text/plain";
	return (tmp);
}

MethodGet::MethodGet(Config _conf, Request _req, Response &_res)
	: AMethod(_conf, _req, _res){};

MethodGet::~MethodGet(){};

Result<int, bool> MethodGet::setContentType(std::string filename) {
	std::vector<std::string> list;
	std::string contenttype;

	std::stringstream ss;
	std::string tmp;
	ss << filename;
	while (std::getline(ss, tmp, '.')) {
		if (tmp.empty() == false) list.push_back(tmp);
	}

	if (ext.find(list.back()) != ext.end())
		contenttype = ext.at(list.back());
	else
		contenttype = DEFAULT_EXT_TYPE;
	res.addHeader("Content-Type", contenttype);
	return (0);
}

Result<int, bool> MethodGet::checkGetSemantics() {
	//条件付きGetかどうかを調べる
	Result<std::string, bool> res_1 = res.getHeader("If-Modified-Since");
	if (res_1.isOK() == true) {
		getsemantics = "If-Modified-Since";
		return Ok<int>(0);
	}
	Result<std::string, bool> res_2 = res.getHeader("If-Unmodified-Since");
	if (res_2.isOK() == true) {
		getsemantics = "If-Unmodified-Since";
		return Ok<int>(0);
	}
	Result<std::string, bool> res_3 = res.getHeader("If-Match");
	if (res_3.isOK() == true) {
		getsemantics = "If-Match";
		return Ok<int>(0);
	}
	Result<std::string, bool> res_4 = res.getHeader("If-None-Match");
	if (res_4.isOK() == true) {
		getsemantics = "If-None-Match";
		return Ok<int>(0);
	}
	Result<std::string, bool> res_5 = res.getHeader("If-Range");
	if (res_5.isOK() == true) {
		getsemantics = "If-Range";
		return Ok<int>(0);
	}

	//部分Getかどうかを調べる
	Result<std::string, bool> res_6 = res.getHeader("Range");
	if (res_6.isOK() == true) {
		getsemantics = "Range";
		return Ok<int>(0);
	}
	getsemantics = "GET";
	return Ok<int>(0);
}

Result<int, bool> MethodGet::checkIsDirlisting() {
	int status;
	struct stat t_stat;

	// std::cout << RED << "uri: " << uri << RESET << std::endl;

	// URIがディレクトリを指しているか確認する
	status = stat(uri.c_str(), &t_stat);
	if (status != 0) {
		// std::cout << "stat failed" << errno << std::endl;
		if (errno == ENAMETOOLONG) {
			res.setStatus(414);
			res.setStatusMessage("URI Too Long");
			setErrorPageBody();
			return Ok<int>(-1);
		} else if (errno == ENOENT) {
#if defined(_DEBUGFLAG)
			std::cout << RED << "MethodGet::checkIsDirlisting stat失敗。ENOENT"
					  << RESET << std::endl;
#endif
			res.setStatus(404);
			res.setStatusMessage("Not Found");
			setErrorPageBody();
			return Ok<int>(-1);
		} else if (errno == EACCES) {
			res.setStatus(403);
			res.setStatusMessage("Forbidden");
			setErrorPageBody();
			return Ok<int>(-1);
		} else {
#if defined(_DEBUGFLAG)
			std::cout
				<< RED
				<< "MethodGet::checkIsDirlisting stat失敗。エラーコードなし"
				<< RESET << std::endl;
			std::cout << RED << "Filename: " << uri << RESET << std::endl;
#endif
			res.setStatus(500);
			res.setStatusMessage("Internal Server Error");
			res.setHeader("Connection", "close");
			setErrorPageBody();
			return Ok<int>(-1);
		}
	}

	// URIが指しているものがディレクトリじゃなかった場合を弾く
	if (S_ISDIR(t_stat.st_mode) == false) {
		return Error<bool>(false);
	}

	// std::cout << "test" << std::endl;

	// URIがディレクトリを指していて、しかもTrailing
	// slashがなかった場合に追加する
	if (uri.find_last_of('/') != uri.size() - 1) {
		uri += '/';
	}

	// Location,Configのいずれかでディレクトリリスティングが有効になっているか確認する
	if (isloc == true && loc.getDirlist() == true) {
		// do nothing
	} else if (conf.getDirlist() == true) {
		// do nothing
	} else {
		//ディレクトリリスティングが無効だった時Indexファイルを検索しに行く
		Result<int, bool> _res_ind = searchIndex();
		if (_res_ind.isOK() == true) {
			//ここでOkを返すとすぐに帰るので、Indexの読み込みでエラーが起こった場合なども
			// Okが帰ってくる(それはエラーページを返すべきだから)
			return Ok<int>(0);
		} else {
			// Indexが存在しなかったときは、Errorで返して、Openで
			//ディレクトリを開こうとさせてエラーで帰ると思う
			return Error<bool>(false);
		}
	}

	// std::cout << "uri: " << uri << std::endl;

	//有効になっていたらメソッドを呼んでそのまま帰る
	MakeDirlistHTML dir(uri, uri_without_root);
	Result<std::string, bool> dir_res = dir.returnHTML();
	if (dir_res.isOK() == true) {
		std::stringstream ss;
		std::string fs;

		res.setStatus(200);
		res.setStatusMessage("OK");
		// std::cout << dir_res.getOk() << std::endl;
		res.setBody(dir_res.getOk());
		ss << dir_res.getOk().length();
		ss >> fs;
		res.addHeader("Content-Length", fs);
		return Ok<int>(0);
	} else {
#if defined(_DEBUGFLAG)
		std::cout
			<< RED
			<< "MethodGet::checkIsDirlisting MakeDirlistHTMLが失敗している"
			<< RESET << std::endl;
		std::cout << RED << "Filename: " << uri << RESET << std::endl;
#endif
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		res.setHeader("Connection", "close");
		setErrorPageBody();
		return Ok<int>(-1);
	}
}

Result<int, bool> MethodGet::searchIndex() {
	int status;
	std::string tmppath;

	// std::cout << "searching index files!!" << std::endl;
	// std::cout << loc.getIndex().size() << std::endl;

	// locationのIndexを見る
	if (isloc == true && loc.getIndex().size() != 0) {
		for (size_t i = 0; i < loc.getIndex().size(); i++) {
			tmppath = ".";
			tmppath += conf.getRootDir();
			if (conf.getRootDir().empty() == false) tmppath += "/";
			tmppath += loc.getRootDir();
			if (loc.getRootDir().empty() == false) tmppath += "/";
			tmppath += loc.getIndex().at(i);

			// std::cout << tmppath << std::endl;

			// openしてみて、成功したらそれを打ち返す
			// 成功しなかったらどんどん次を読みこむ
			status = open(tmppath.c_str(), O_RDONLY);
			if (status != -1) {
				close(status);
				Result<std::string, bool> const res_read = _openFile(tmppath);
				if (res_read.isError() == true) {
					setErrorPageBody();
					return Ok<int>(0);
				} else {
					res.setStatus(200);
					res.setStatusMessage("OK");
					return Ok<int>(0);
				}
			}
		}
	}

	// ConfigのIndexを見る
	if (conf.getIndex().size() != 0) {
		for (size_t i = 0; i < conf.getIndex().size(); i++) {
			tmppath = ".";
			tmppath += conf.getRootDir();
			if (conf.getRootDir().empty() == false) tmppath += "/";
			tmppath += conf.getIndex().at(i);

			// std::cout << tmppath << std::endl;

			// openしてみて、成功したらそれを打ち返す
			// 成功しなかったらどんどん次を読みこむ
			status = open(tmppath.c_str(), O_RDONLY);
			if (status != -1) {
				close(status);
				Result<std::string, bool> const res_read_c = _openFile(tmppath);
				if (res_read_c.isError() == true) {
					setErrorPageBody();
					return Ok<int>(0);
				} else {
					res.setStatus(200);
					res.setStatusMessage("OK");
					return Ok<int>(0);
				}
			}
		}
	}

	tmppath = ".";
	tmppath += conf.getRootDir();
	tmppath += "/";
	tmppath += "index.html";
	// std::cout << tmppath << std::endl;

	// index.htmlが存在するか調べる(デフォルトの挙動)
	status = open(tmppath.c_str(), O_RDONLY);
	if (status != -1) {
		close(status);
		Result<std::string, bool> const res_read_i = _openFile(tmppath);
		if (res_read_i.isError() == true) {
			setErrorPageBody();
			return Ok<int>(0);
		} else {
			res.setStatus(200);
			res.setStatusMessage("OK");
			return Ok<int>(0);
		}
	}

	//存在しなかったときはErrorのTrueで返す
	return Error<bool>(false);
}

Result<int, bool> MethodGet::act() {

	// AllowedMethodのチェック
	Result<int, bool> const res_alm = isAllowedMethod();
	if (res_alm.isError() == true)
	{
		setErrorPageBody();
		return Error<bool>(false);
	}

	//(ディレクトリリスティングが有効なら)もしくは(この内部処理でエラーが起こっていた→エラーページをセットして)この時点で帰る
	Result<int, bool> res_dir = checkIsDirlisting();
	if (res_dir.isOK() == true) {
		return Ok<int>(0);
	}

	// Getの条件を確認する
	checkGetSemantics();

	// 拡張子を見てContentTypeを判断しResponseにセット
	setContentType(uri);

	// ファイルの中身を読み込んでBodyに詰める
	Result<std::string, bool> const res_read = _openFile(uri);
	if (res_read.isError() == true) {
		setErrorPageBody();
		return Error<bool>(false);
	} else {
		res.setStatus(200);
		res.setStatusMessage("OK");
		return Ok<int>(0);
	}
	return Ok<int>(0);
}
