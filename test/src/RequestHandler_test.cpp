/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:17:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/21 10:27:44 by komatsud         ###   ########.fr       */
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

//ポート番号の有無などの差にも対応するようにする
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

TEST(RequestHandlerTest, checkRequiedHeaderTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);
}

TEST(RequestHandlerTest, checkRequiedHeaderTest_Error_HTTPVersion)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(505);
	std::string					expected_string("HTTP Version Not Supported");

	req.setVersion("HTTP/2.0");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST(RequestHandlerTest, checkRequiedHeaderTest_Error_HTTPVersion)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(505);
	std::string					expected_string("HTTP Version Not Supported");

	req.setVersion("HTTP/2.0");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

