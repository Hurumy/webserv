/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodDelete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:17:40 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/27 19:07:03 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodDelete.hpp"

MethodDelete::MethodDelete(Config _conf, Request _req, Response &_res)
	: AMethod(_conf, _req, _res) {}

MethodDelete::~MethodDelete() {}

int MethodDelete::deleteFile() {
	int status;

	status = unlink(uri.c_str());
	if (status == -1) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}
	res.setStatus(204);
	res.setStatusMessage("No Content");
	return (204);
}

// getUrlを後でいい感じの変換後のパスに置き換えたい
int MethodDelete::openResourceDelete() {
	int status;

	//存在するかどうか
	status = access(uri.c_str(), F_OK);
	if (status == 0) {
		//存在した場合、書き込み許可があるかどうか
		status = access(uri.c_str(), W_OK);
		if (status != -1) {
			status = deleteFile();
			return (status);
		} else {
			res.setStatus(401);
			res.setStatusMessage("Unauthorized");
			return (401);
		}
	} else {
		res.setStatus(404);
		res.setStatusMessage("Not Found");
		return (404);
	}
}

Result<int, bool> MethodDelete::act() {
	int status;

	//URIを確認します
	Result<int, bool>	res_uri = checkURI();
	if (res_uri.isOK() == false)
	{
		setErrorPageBody();
		return Error<bool>(false);
	}
	setURI();

	status = openResourceDelete();
	if (200 <= status && status <= 299)
		return Ok<int>(status);
	else
		return Error<bool>(false);
}
