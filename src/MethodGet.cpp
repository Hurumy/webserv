/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:09:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 11:04:05 by komatsud         ###   ########.fr       */
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

MethodGet::MethodGet(Config _conf, Request _req, Response &_res)
	: AMethod(_conf, _req, _res){};

MethodGet::~MethodGet(){};

Result<int, bool> MethodGet::setContentType(std::string filename) {
	std::map<std::string, std::string> ext;
	std::vector<std::string> list;
	std::string contenttype;

	ext["html"] = "text/html";
	ext["csv"] = "text/csv";
	ext["css"] = "text/css";
	ext["js"] = "text/javascript";
	ext["json"] = "application/json";
	ext["pdf"] = "application/pdf";
	ext["xls"] = "application/vnd.ms-excel";
	ext["xlsx"] =
		"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	ext["ppt"] = "application/vnd.ms-powerpoint";
	ext["pptx"] =
		"application/"
		"vnd.openxmlformats-officedocument.presentationml.presentation";
	ext["doc"] = "application/msword";
	ext["docx"] =
		"application/"
		"vnd.openxmlformats-officedocument.wordprocessingml.document";
	ext["jpg"] = "image/jpeg";
	ext["jpeg"] = "image/jpeg";
	ext["png"] = "image/png";
	ext["gif"] = "image/gif";
	ext["bmp"] = "image/bmp";
	ext["dib"] = "image/bmp";
	ext["svg"] = "image/svg+xml";
	ext["zip"] = "application/zip";
	ext["lzh"] = "application/x-lzh";
	ext["tar"] = "application/x-tar";
	ext["gz"] = "application/x-tar";
	ext["mp3"] = "audio/mpeg";
	ext["mp4"] = "video/mp4";
	ext["mpeg"] = "video/mpeg";
	ext["exe"] = "application/octet-stream";
	ext["out"] = "application/octet-stream";
	ext["txt"] = "text/plain";

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

Result<int, bool> MethodGet::act() {
	std::string filename = req.getUrl();

	// Getの条件を確認する
	checkGetSemantics();

	//拡張子を見てContentTypeを判断しResponseにセット
	setContentType(filename);

	//ファイルの中身を読み込んでBodyに詰める
	Result<std::string, bool> const res_read = _openFile(filename);
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
