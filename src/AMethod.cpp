/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:41:01 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 16:26:49 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

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
		res.setStatusMessage("Not Found");
		return Error<bool>(false);
	} else if (fd == -1 && errno == EACCES) {
		res.setStatus(403);
		res.setStatusMessage("Forbidden");
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
		res.setStatusMessage("Internal Server Error");
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

void AMethod::setErrorPageBody() {
	unsigned int prevstatus = res.getStatus();
	Result<std::string, bool> res_1 = conf.getErrorPages(res.getStatus());

	if (res_1.isOK() == false) return;

	std::string filename = res_1.getOk();

	// bodyをセットする。成功したら抜けるループ
	// bodyのセットに失敗した場合は、失敗した後のステータスが変わっていればもう一度ファイルを検索する
	//変わっていなければBodyなしでヘッダだけ送付する
	while (1) {
		Result<std::string, bool> res_2 = _openFile(filename);
		if (res_2.isOK() == true)
			break;
		else if (res_2.isOK() == false && prevstatus != res.getStatus()) {
			prevstatus = res.getStatus();
			Result<std::string, bool> res_3 =
				conf.getErrorPages(res.getStatus());
			if (res_3.isOK() == false) break;
			filename = res_3.getOk();
		} else
			break;
	}
	return;
}
