/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:32:21 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 13:57:35 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "AMethod.hpp"
#include "MethodGet.hpp"
#include "MethodPost.hpp"

RequestHandler::RequestHandler(std::vector<Config> const _conf, Request const _req)
{
	this->configs = _conf;
	this->req = _req;
	this->res = Response();
}

//configs.at(i) の i を返す
Result<int, bool>	RequestHandler::searchMatchHost()
{
	Result<std::string, bool> 	result_1 = req.getHeader("Host");
	std::string					hostname;

	if (result_1.isOK() == false)
	{
		res.setStatus(400);
		res.setStatusMessage("Bad Request");
		return Error<bool>(false);
	}

	hostname = result_1.getOk();

	for (size_t i = 0; i < configs.size(); i ++)
	{
		for (size_t t = 0; t < configs.at(i).getServerName().size(); t ++)
		{
			if (configs.at(i).getServerName().at(t) == hostname)
			{
				this->confnum = i;
				return Ok<int>(i);
			}
		}
	}
	res.setStatus(400);
	res.setStatusMessage("Bad Request");
	return Error<bool>(false);
}

Result<int, bool>	RequestHandler::checkRequiedHeader()
{
	if (req.getVersion() != "HTTP/1.1")
	{
		res.setStatus(505);
		res.setStatusMessage("HTTP Version Not Supported");
		return Error<bool>(false);
	}
	if (this->confnum < configs.size() && configs.at(confnum).getReqMethod(req.getMethod()).isOK() == false)
	{
		res.setStatus(405);
		res.setStatusMessage("Method Not Allowed");
		return Error<bool>(false);
	}
	return Ok<int>(0);
}

Result<int, bool>	RequestHandler::routeMethod()
{
	if (req.getMethod() == "GET")
	{
		//クラス呼ぶ
		MethodGet get(configs.at(confnum), req, res);
		Result<int, bool> res_get = get.act();
		if (res_get.isOK() == false)
			return Error<bool>(false);
		else
			return Ok<int>(0);
	}
	else if (req.getMethod() == "POST")
	{
		//クラス呼ぶ
		MethodPost post(configs.at(confnum), req, res);
		Result<int, bool> res_get = post.act();
		if (res_get.isOK() == false)
			return Error<bool>(false);
		else
			return Ok<int>(0);
		
	}
	else if (req.getMethod() == "DELETE")
	{
		//クラス呼ぶ
	}
	else
	{
		res.setStatus(405);
		res.setStatusMessage("Method Not Allowed");
		return Error<bool>(false);
	}
	return Error<bool>(false);
}

Result<std::string, bool>	RequestHandler::_openFile(std::string filename)
{
	int 				fd;
	unsigned long long	bodysize = 0;
	int 				status = 1;
	std::string 		body;
	char 				buf[FILE_READ_SIZE];

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
		buf[status] = '\0';
		if (status != -1)
		{
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

	//Bodyの読み込みが成功していたら、bodysizeとBodyをセットして返る
	std::stringstream		ss;
	std::string				length;
	ss << bodysize;
	ss >> length;
	res.addHeader("Content-Length", length);
	res.setBody(body);
	return Ok<std::string>(body);
}

void		RequestHandler::setErrorPageBody()
{
	unsigned int				prevstatus = res.getStatus();
	Result<std::string, bool>	res_1 = configs.at(confnum).getErrorPages(res.getStatus());
	
	if (res_1.isOK() == false)
		return ;

	std::string					filename = res_1.getOk();

	//bodyをセットする。成功したら抜けるループ
	//bodyのセットに失敗した場合は、失敗した後のステータスが変わっていればもう一度ファイルを検索する
	//変わっていなければBodyなしでヘッダだけ送付する
	while (1)
	{
		Result<std::string, bool> res_2 = _openFile(filename);
		if (res_2.isOK() == true)
			break ;
		else if (res_2.isOK() == false && prevstatus != res.getStatus())
		{
			prevstatus = res.getStatus();
			Result<std::string, bool>	res_3 = configs.at(confnum).getErrorPages(res.getStatus());
			if (res_3.isOK() == false)
				break ;
			filename = res_3.getOk();
		}
		else
			break ;
	}
	return ;
}

Response	RequestHandler::getResponse()
{
	return (this->res);
}


