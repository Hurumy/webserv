/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:09:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/28 11:31:45 by komatsud         ###   ########.fr       */
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

const	std::map<std::string, std::string>	MethodGet::ext = initExtMap();

std::map<std::string, std::string>	MethodGet::initExtMap()
{
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

Result<int, bool> MethodGet::act() {

	//URIを確認します
	Result<int, bool>	res_uri = checkURI();
	if (res_uri.isOK() == false)
	{
		setErrorPageBody();
		return Error<bool>(false);
	}
	setURI();

	// Getの条件を確認する
	checkGetSemantics();

	//拡張子を見てContentTypeを判断しResponseにセット
	setContentType(uri);

	//リダイレクトなどを確認する

	//ファイルの中身を読み込んでBodyに詰める
	Result<std::string, bool> const res_read = _openFile(uri);
	if (res_read.isError() == true) {
		setErrorPageBody();
		return Error<bool>(false);
	}
	else
	{
		res.setStatus(200);
		res.setStatusMessage("OK");
		return Ok<int>(0);
	}
	return Ok<int>(0);
}
