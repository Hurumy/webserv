/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_put.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 08:23:35 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/06 16:52:55 by komatsud         ###   ########.fr       */
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

static int putCreateFile(Request req, Response &res) {
	int fd;
	int status;

	std::cout << req.getUrl() << std::endl;
	std::cout << YELLOW "Body: " << req.getBody() << RESET << std::endl;
	fd = open(req.getUrl().substr(1, req.getUrl().size()).c_str(),
			  O_WRONLY | O_CREAT, 0777);
	if (fd < 0) {
		std::cout << RED "open failed" RESET << std::endl;
		std::cout << RED "errno: " << errno << RESET << std::endl;
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}
	status = writeToFile(req, res, fd);
	if (status == 0)
		return (201);
	else
		return (status);
}

static int putReplaceFile(Request req, Response &res) {
	int fd;
	int status;

	std::cout << req.getUrl() << std::endl;
	fd = open(req.getUrl().substr(1, req.getUrl().size()).c_str(),
			  O_WRONLY | O_TRUNC);
	if (fd == -1) {
		std::cout << RED "open failed" RESET << std::endl;
		std::cout << RED "errno: " << errno << RESET << std::endl;
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}
	status = writeToFile(req, res, fd);
	if (status == 0)
		return (204);
	else
		return (status);
}

// getUrlを後でいい感じの変換後のパスに置き換えたい
static int openResourcePut(Request req, Response &res) {
	int status;

	//存在するかどうか
	status = access(req.getUrl().substr(1, req.getUrl().size()).c_str(), F_OK);
	if (status == 0) {
		//存在した場合、書き込み許可があるかどうか
		status =
			access(req.getUrl().substr(1, req.getUrl().size()).c_str(), W_OK);
		if (status != -1) {
			res.setStatus(204);
			res.setStatusMessage("No Content");
			status = putReplaceFile(req, res);
			return (status);
		} else {
			res.setStatus(401);
			res.setStatusMessage("Unauthorized");
			return (401);
		}
	} else {
		res.setStatus(201);
		res.setStatusMessage("Created");
		status = putCreateFile(req, res);
		return (status);
	}
}

//指定されたURLにBodyの内容のファイルを作る
//すでにあったらBodyの内容で置換する(更新とかではない)
Result<size_t, int> methodPut(Request req, Response &res) {
	int status;

	status = openResourcePut(req, res);
	if (status == 201 || status == 204 || status == 200)
		return Ok<size_t>(status);
	else
		return Error<int>(status);
}
