/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initRes_isMatchHost.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:07:36 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/20 13:43:08 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initResponse.hpp"

//configs.at(i) の i を返す
Result<int, bool>	isMatchHost(Response &res, Request req, std::vector<Config> configs)
{
	size_t						index;
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
				return Ok<int>(i);
		}
	}
	return Error<bool>(false);
}
