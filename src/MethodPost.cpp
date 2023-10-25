/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:24:13 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/23 14:55:46 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodPost.hpp"

#include "AMethod.hpp"
#include "webserv.hpp"

MethodPost::MethodPost(Config _conf, Request _req, Response &_res)
	: AMethod(_conf, _req, _res) {}

MethodPost::~MethodPost() {}

int MethodPost::writeToFile(int fd) {
	int status;
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
	size_t i = filesize;
	while (i > 0) {
		status = write(fd, req.getBody().c_str(), i);
		if (status == -1) {
			res.setStatus(500);
			res.setStatusMessage("Internal Server Error");
			return (500);
		}
		i = i - status;
	}

	// FDを閉じる
	status = close(fd);
	if (status == -1) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}
	return (0);
}

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

Result<int, bool> MethodPost::checkMaxBodySize()
{
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

	//locationのMaxBodySizeを見る
	if (filesize > loc.getMaxBodySize())
	{
		res.setStatus(413);
		res.setStatusMessage("Payload Too Large");
		return Error<bool>(false);
	}
	//configのMaxBodySizeを見る
	else if (filesize > conf.getMaxBodySize())
	{
		res.setStatus(413);
		res.setStatusMessage("Payload Too Large");
		return Error<bool>(false);
	}

	return Ok<int>(0);
}

int MethodPost::openPostResource() {
	int status = 0;
	int fd;
	std::string	uppath;

	//locationでUploadPathが設定されていたらそれを使う
	if (isloc == true && loc.getUploadPath().empty() == false)
	{
		uppath = loc.getUploadPath();
	}
	//configでUploadPathが設定されていたらそれを使う
	else if (conf.getUploadPath().empty() == false)
	{
		uppath = conf.getUploadPath();
	}
	//どちらも設定がなければURIをそのまま使う
	else
	{
		uppath = uri;
	}

	//パス自体へのアクセスを調べる
	status = access(uppath.c_str(), W_OK);
	if (status == -1) {
		#if defined(_DEBUGFLAG)
			std::cout << RED << "Error in MethodPost::openPostResource" << RESET << std::endl;
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
	status = open(filename.c_str(), O_WRONLY | O_CREAT, 0777);
	if (status == -1) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	} else {
		res.setStatus(201);
		res.setStatusMessage("Created");
	}

	//書き込む
	fd = status;
	status = writeToFile(fd);
	if (status == 0)
		return (201);
	else
		return (status);
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
