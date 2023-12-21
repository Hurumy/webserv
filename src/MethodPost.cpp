/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:24:13 by komatsud          #+#    #+#             */
/*   Updated: 2023/12/11 15:29:54 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodPost.hpp"

#include "AMethod.hpp"
#include "webserv.hpp"

const std::map<std::string, std::string> MethodPost::ext = initExtMap();

std::map<std::string, std::string> MethodPost::initExtMap() {
	std::map<std::string, std::string> tmp;
	tmp["text/html"] = "html";
	tmp["text/csv"] = "csv";
	tmp["text/css"] = "css";
	tmp["text/javascript"] = "js";
	tmp["application/json"] = "json";
	tmp["application/pdf"] = "pdf";
	tmp["application/vnd.ms-excel"] = "xls";
	tmp["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] =
		"xlsx";
	tmp["application/vnd.ms-powerpoint"] = "ppt";
	tmp["application/"
		"vnd.openxmlformats-officedocument.presentationml.presentation"] =
		"pptx";
	tmp["application/msword"] = "doc";
	tmp["application/"
		"vnd.openxmlformats-officedocument.wordprocessingml.document"] = "docx";
	tmp["image/jpeg"] = "jpg";
	tmp["image/png"] = "png";
	tmp["image/gif"] = "gif";
	tmp["image/bmp"] = "bmp";
	tmp["image/svg+xml"] = "svg";
	tmp["application/zip"] = "zip";
	tmp["application/x-lzh"] = "lzh";
	tmp["application/x-tar"] = "tar";
	tmp["audio/mpeg"] = "mp3";
	tmp["video/mp4"] = "mp4";
	tmp["video/mpeg"] = "mpeg";
	tmp["application/octet-stream"] = "exe";
	tmp["text/plain"] = "txt";
	return (tmp);
}

MethodPost::MethodPost(Config _conf, Request _req, Response &_res)
	: AMethod(_conf, _req, _res) {}

MethodPost::~MethodPost() {}

std::string MethodPost::makeFilename(std::string _uppath) {
	int status = 0;
	unsigned long long number = 0;
	std::stringstream ss;
	std::string _ext = "";
	bool is_type = false;

	Result<std::string, bool> res_type = req.getHeader("Content-Type");
	if (res_type.isOK() == true) {
		std::string type = res_type.getOk();
		if (ext.find(type) != ext.end()) {
			_ext = ext.at(type);
			is_type = true;
		}
	}

	do {
		ss.str("");
		ss.clear(std::stringstream::goodbit);
		ss << _uppath;
		ss << "/";
		ss << number;
		if (is_type == true) {
			ss << ".";
			ss << _ext;
		}
		filename = ss.str();
		status = access(filename.c_str(), F_OK);
		number++;
	} while (status != -1);
	return (filename);
}

Result<int, bool> MethodPost::checkMaxBodySize() {
	std::stringstream ss;
	unsigned long long filesize;

	Result<std::string, bool> header = req.getHeader("Content-Length");

	if (header.isOK() == false) {
		res.setStatus(411);
		res.setStatusMessage("Length Required");
		return Error<bool>(false);
	}

	ss << header.getOk();
	ss >> filesize;

	// locationのMaxBodySizeを見る
	if (isloc && filesize > loc.getMaxBodySize()) {
#if defined(_DEBUGFLAG)
		std::cout << RED "filesize: " << filesize << std::endl;
		std::cout << "location: " << loc.getMaxBodySize() << RESET << std::endl;
#endif
		res.setStatus(413);
		res.setStatusMessage("Payload Too Large");
		return Error<bool>(false);
	}
	// configのMaxBodySizeを見る
	else if (filesize > conf.getMaxBodySize()) {
#if defined(_DEBUGFLAG)
		std::cout << RED "filesize: " << filesize << std::endl;
		std::cout << "config: " << conf.getMaxBodySize() << RESET << std::endl;
#endif
		res.setStatus(413);
		res.setStatusMessage("Payload Too Large");
		return Error<bool>(false);
	}

	return Ok<int>(0);
}

int MethodPost::openPostResource() {
	int status = 0;
	std::string uppath;

	// locationでUploadPathが設定されていたらそれを使う
	if (isloc == true && loc.getUploadPath().empty() == false) {
		uppath = loc.getUploadPath();
	}
	// configでUploadPathが設定されていたらそれを使う
	else if (conf.getUploadPath().empty() == false) {
		uppath = conf.getUploadPath();
	}
	//どちらも設定がなければURIをそのまま使う
	else {
		uppath = uri;
	}

	//パス自体へのアクセスを調べる
	status = access(uppath.c_str(), W_OK);
	if (status == -1) {
#if defined(_DEBUGFLAG)
		std::cout << RED << "Error in MethodPost::openPostResource" << RESET
				  << std::endl;
		std::cout << RED << "uppath: " << uppath << RESET << std::endl;
#endif
		res.setStatus(401);
		res.setStatusMessage("Unauthorized");
		return (401);
	}

	// Config/Locationで指定されているMaxBodySizeを超えていないかしらべる
	Result<int, bool> res_size = checkMaxBodySize();
	if (res_size.isOK() == false) return (413);

	//被りのないファイル名を調べる
	filename = makeFilename(uppath);
	// std::cout << BLUE << filename << RESET << std::endl;

	//作ったファイル名のファイルを開く
	std::ofstream ofs(filename.c_str(), std::ios::binary);
	if (!ofs) {
#if defined(_DEBUGFLAG)
		std::cout
			<< RED
			<< "Error in MethodPost::openPostResource Internal Server Error"
			<< RESET << std::endl;
		std::cout << RED << "uppath: " << uppath << RESET << std::endl;
#endif
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		res.setHeader("Connection", "close");
		return (500);
	} else {
		res.setStatus(201);
		res.setStatusMessage("Created");
	}

	//書き込む
	std::stringstream ss;
	std::string str;
	unsigned long long filesize;

	//ファイルサイズを取得する
	Result<std::string, bool> result = req.getHeader("Content-Length");
	if (result.isOK() == true)
		str = result.getOk();
	else {
		res.setStatus(411);
		res.setStatusMessage("Length Required");
		return (411);
	}
	ss << str;
	ss >> filesize;

	// std::cerr << RED << "filesize: " << filesize << RESET << std::endl;

	// Content-Lengthとボディのサイズが異なった場合400で返す
	if (filesize != req.getBody().size()) {
#if defined(_DEBUGFLAG)
		std::cerr << RED
				  << "Error::MethodPost::openPostResource::400::"
					 "ContentーLengthとボディのサイズが一致しない"
				  << RESET << std::endl;
#endif
		res.setStatus(400);
		res.setStatusMessage("Bad Request");
		return (400);
	}

	//ファイルに書き込みをする
	for (unsigned long long i = 0; i < filesize / sizeof(char); i++) {
		ofs.write(&req.getBody().c_str()[i], sizeof(char));
		// std::cerr << YELLOW << "i: " << i << RESET << std::endl;
	}

	ofs.close();

	// ofsを閉じる
	if ((ofs.rdstate() & std::ios_base::failbit) != 0 ||
		(ofs.rdstate() & std::ios_base::badbit) != 0) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		res.setHeader("Connection", "close");
		return (500);
	}

	return (201);
}

Result<int, bool> MethodPost::act() {
	int status;

	status = openPostResource();
	if (status >= 200 && status <= 299) {
		res.addHeader("Location", filename.substr(1, filename.size() - 1));
		res.addHeader("Content-Length", "0");
		return Ok<int>(status);
	} else {
		setErrorPageBody();
		return Error<bool>(false);
	}
};
