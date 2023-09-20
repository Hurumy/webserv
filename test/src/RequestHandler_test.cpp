/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:17:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/20 18:27:28 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "RequestHandler.hpp"
#include "webserv.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "ConfParser.hpp"

#define	CONF_FILE_PATH "testconfs/simple.conf"

TEST(RequestHandlerTest, searchMatchHostTest)
{
	std::vector<Config>			tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request						req;
	int							expected(1);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.getOk(), expected);
}


