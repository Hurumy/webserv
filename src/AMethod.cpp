/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:41:01 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/02 14:33:52 by komatsud         ###   ########.fr       */
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
	// 変わっていなければBodyなしでヘッダだけ送付する
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
		} else {
			res.addHeader("Content-Length", "0");
			break;
		}
	}
	return;
}

Result<int, bool> AMethod::checkURI() {
	std::string origin = req.getUrl();
	std::string rel;

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
		res.setStatus(400);
		res.setStatusMessage("Bad Request");
		return Error<bool>(false);
	}

	//　サーバー自体のルートより上を見ようとしていないか、
	//　変な指定がないかなどのチェックを足す

	uri = rel;
	return Ok<int>(0);
}

void AMethod::setURI() {
	std::string tmp;

	// ROOTを見る
	if (conf.getRootDir().empty() == false) {
		tmp = conf.getRootDir() + uri;
		uri = tmp;
	}

	//最初に.をつけて開けるようにする
	tmp = "." + uri;
	uri = tmp;

	// std::cout << YELLOW << uri << RESET << std::endl;

	return;
}

// Result<int, bool>	AMethod::searchSettingsOfURI()
// {
// 	//return
// 	if (conf.isReturn() == true)
// 	{
// 		if (conf.getReturnUrl().empty() == false)
// 		{

// 		}
// 		else
// 		{
// 			res.setStatus(conf.getReturnStatus());

// 		}
// 	}

// 	//rewrite

// }
