/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_delete.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 14:39:30 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/06 16:50:09 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "APayload.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "Result.hpp"

static int	deleteFile(Request req, Response &res)
{
	int status;

	status = unlink(req.getUrl().substr(1, req.getUrl().size()).c_str());
	if (status == -1)
	{
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return (500);
	}
	res.setStatus(204);
	res.setStatusMessage("No Content");
	return (204);
}

//getUrlを後でいい感じの変換後のパスに置き換えたい
static int	openResourceDelete(Request req, Response &res)
{
	int	status;

	//存在するかどうか
	status = access(req.getUrl().substr(1, req.getUrl().size()).c_str(), F_OK);
	if (status == 0)
	{
		//存在した場合、書き込み許可があるかどうか
		status = access(req.getUrl().substr(1, req.getUrl().size()).c_str(), W_OK);
		if (status != -1)
		{
			status = deleteFile(req, res);
			return (status);		
		}
		else
		{
			res.setStatus(401);
			res.setStatusMessage("Unauthorized");	
			return (401);
		}
	}
	else
	{
		res.setStatus(404);
		res.setStatusMessage("Not Found");
		return (404);
	}
}

//指定されたURLにBodyの内容のファイルを作る
//すでにあったらBodyの内容で置換する(更新とかではない)
Result<size_t, int>	methodDelete(Request req, Response &res)
{
	int	status;

	status = openResourceDelete(req, res);
	if (200 <= status && status <= 299)
		return Ok<size_t>(status);
	else
		return Error<int>(status);
}

