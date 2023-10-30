/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:24:13 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/30 14:35:40 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodPost.hpp"

#include "AMethod.hpp"
#include "webserv.hpp"

MethodPost::MethodPost(Config _conf, Request _req, Response &_res)
	: AMethod(_conf, _req, _res) {}

MethodPost::~MethodPost() {}

std::string MethodPost::makeFilename(std::string _uppath) {
	int status = 0;
	unsigned long long number = 0;
	std::stringstream ss;

	do {
		ss.str("");
		ss.clear(std::stringstream::goodbit);
		ss << _uppath;
		ss << "/";
		ss << number;
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
	if (filesize > loc.getMaxBodySize()) {
		std::cout << "location: " << loc.getMaxBodySize() << std::endl;
		res.setStatus(413);
		res.setStatusMessage("Payload Too Large");
		return Error<bool>(false);
	}
	// configのMaxBodySizeを見る
	else if (filesize > conf.getMaxBodySize()) {
		std::cout << "config: " << conf.getMaxBodySize() << std::endl;
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
	std::ofstream	ofs(filename, std::ios::binary);
	if (!ofs) {
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

	//ファイルに書き込みをする
	ofs.write(req.getBody().c_str(), sizeof(req.getBody().c_str()));

	// ofsを閉じる
	if ((ofs.rdstate() & std::ios_base::failbit) != 0 || (ofs.rdstate() & std::ios_base::badbit) != 0) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		res.setHeader("Connection", "close");
		return (500);
	}

	ofs.close();
	
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
