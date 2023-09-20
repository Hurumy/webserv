/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:15:29 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/20 13:44:26 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "APayload.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Error.hpp"
#include "Ok.hpp"

Result<int, bool>	isMatchHost(Response &res, Request req, std::vector<Config> configs);
Result<std::string, bool>	checkRequiredHeader(Request req, Response &res, Config conf);
