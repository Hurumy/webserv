/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:15:29 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 13:02:35 by komatsud         ###   ########.fr       */
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

Result<std::string, bool>	checkRequiredHeader(Request req, Response &res, Config conf);
