/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_post.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:08:46 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/06 16:54:20 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

static int writeToFile(Request req, Response &res, int fd) {
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
	status = write(fd, req.getBody().c_str(), filesize);
	if (status == -1) {
		std::cout << RED "write failed" RESET << std::endl;
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}

	// FDを閉じる
	status = close(fd);
	if (status == -1) {
		std::cout << RED "close failed" RESET << std::endl;
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}
	return (0);
}

static std::string makeFilename(Request req) {
	int status = 0;
	unsigned long long number = 0;
	std::stringstream ss;
	std::string filename;

	do {
		ss.str("");
		ss.clear(std::stringstream::goodbit);
		ss << req.getUrl().substr(1, req.getUrl().size());
		ss << "/";
		ss << number;
		filename = ss.str();
		status = access(filename.c_str(), F_OK);
		number++;
	} while (status != -1);
	return (filename);
}

// getUrlを後でいい感じの変換後のパスに置き換えたい
static int openPostResource(Request req, Response &res) {
	int status = 0;
	int fd;
	std::string filename;

	//パス自体へのアクセスを調べる
	status = access(req.getUrl().substr(1, req.getUrl().size()).c_str(), W_OK);
	if (status == -1) {
		res.setStatus(401);
		res.setStatusMessage("Unauthorized");
		return (401);
	}

	//被りのないファイル名を調べる
	filename = makeFilename(req);

	//作ったファイル名のファイルを開く
	status = open(filename.c_str(), O_WRONLY | O_CREAT, 0777);
	std::cout << YELLOW "reqBody: " << req.getBody() << RESET << std::endl;
	if (status == -1) {
		std::cout << RED "open failed" RESET << std::endl;
		std::cout << RED "errno: " RESET << std::endl;
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	} else {
		res.setStatus(201);
		res.setStatusMessage("Created");
	}

	//書き込む
	fd = status;
	status = writeToFile(req, res, fd);
	if (status == 0)
		return (201);
	else
		return (status);
}

//指定されたディレクトリの下に勝手に名前をつけてファイルを置く
Result<size_t, int> methodPost(Request req, Response &res) {
	int status;

	status = openPostResource(req, res);
	if (status >= 200 && status <= 299)
		return Ok<size_t>(status);
	else
		return Error<int>(status);
}
