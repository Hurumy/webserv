/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initRes_checkRequiedHeader.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:40:45 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 13:02:26 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initResponse.hpp"

Result<std::string, bool>	checkRequiredHeader(Request req, Response &res, Config conf)
{
	if (req.getVersion() != "HTTP/1.1")
	{
		res.setStatus(505);
		res.setStatusMessage("HTTP Version Not Supported");
		return Error<bool>(false);
	}
	if (conf.getReqMethod(req.getMethod()).isOK() == false)
	{
		res.setStatus(405);
		res.setStatusMessage("Method Not Allowed");
		return Error<bool>(false);
	}
}
