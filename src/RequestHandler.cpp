/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:32:21 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/21 10:25:13 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

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
	}
	else if (req.getMethod() == "POST")
	{
		//クラス呼ぶ
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

Response	RequestHandler::getResponse()
{
	return (this->res);
}


