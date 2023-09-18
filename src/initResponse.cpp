/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:13:16 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 12:58:58 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initResponse.hpp"

std::vector<std::string>	genHeadersArray(void)
{
	std::vector<std::string> res;

	res.push_back("Cache-Control");
	res.push_back("Connection");
	res.push_back("Date");
	res.push_back("Pragma");
	res.push_back("Transfer-encoding");
	res.push_back("Upgrade");
	res.push_back("Via");
	return (res);
}

std::vector<std::string>	reqHeadersArray(void)
{
	std::vector<std::string>	res;

	res.push_back("Accept");
	res.push_back("Accept-Charset");
	res.push_back("Accept-Encoding");
	res.push_back("Accept-Language");
	res.push_back("Authorization");
	res.push_back("From");
	res.push_back("Host");
	res.push_back("If-Modified-Since");
	res.push_back("If-Match");
	res.push_back("If-None-Match");
	res.push_back("If-Range");
	res.push_back("If-Unmodified-Since");
	res.push_back("Max-Forwards");
	res.push_back("Proxy-Authorization");
	res.push_back("Range");
	res.push_back("Referer");
	res.push_back("User-Agent");
	return (res);
}

std::vector<std::string>	resHeadersArray(void)
{
	std::vector<std::string>	res;

	res.push_back("Age");
	res.push_back("Location");
	res.push_back("Proxy-Authenticate");
	res.push_back("Public");
	res.push_back("Retry-After");
	res.push_back("Server");
	res.push_back("Vary");
	res.push_back("Warning");
	res.push_back("WWW-Authenticate");
	return (res);
}

std::vector<std::string>	entHeadersArray(void)
{
	std::vector<std::string>	res;

	res.push_back("Allow");
	res.push_back("Content-Base");
	res.push_back("Content-Encoding");
	res.push_back("Content-Language");
	res.push_back("Content-Length");
	res.push_back("Content-Location");
	res.push_back("Content-MD5");
	res.push_back("Content-Range");
	res.push_back("Content-Type");
	res.push_back("ETag");
	res.push_back("Expires");
	res.push_back("Last-Modified");
	return (res);
}

Response	initResponse(Response &res, Request req, std::vector<Config> configs)
{
	Config	conf;


	//requestの中身を参照しつつ、一つ一つ詰める
	//エラーが発生していた場合、その場所でステータスコードなどを詰めて帰って来るので、そのまま返る


	//Hostをチェックして該当するConfがあることを確かめる
	
	
	//必須のヘッダがあることを確かめる
	Result<std::string, bool> res_1 = checkRequiredHeader(req, res, conf);
	if (res_1.isOK() == false)
		return (res);

	//入りうるヘッダを全て見て、それぞれ対応する

	//拾った情報に合わせてレスポンスを作る

}


