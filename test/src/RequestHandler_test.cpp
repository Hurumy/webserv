/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:17:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/21 18:34:28 by komatsud         ###   ########.fr       */
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

TEST(RequestHandlerTest, searchMatchHostTest_Error_WrongHost)
{
	std::vector<Config>			tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(400);
	std::string					expected_string("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "wronghost.test.net");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_NoHostHeaderInRequest)
{
	std::vector<Config>			tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(400);
	std::string					expected_string("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
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

TEST(RequestHandlerTest, checkRequiedHeaderTest_Error_AllowedMethod)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(405);
	std::string					expected_string("Method Not Allowed");

	req.setVersion("HTTP/1.1");
	req.setMethod("PET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST(RequestHandlerTest, getResponseTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(405);
	std::string					expected_string("Method Not Allowed");

	req.setVersion("HTTP/1.1");
	req.setMethod("PET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST(RequestHandlerTest, setErrorPageBodyTest_Error_HTTPVersion)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(505);
	std::string					expected_string("HTTP Version Not Supported");
	std::string					expected_body("<H1>HTTP 505 HTTP Version Not Supported</H1>\n\n<p>This webserv supports only HTTP/1.1...</p>");
	std::string					expected_content_length("91");

	req.setVersion("HTTP/2.0");
	req.setMethod("GET");
	req.addHeader("Host", "_");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	//errorPageBody
	handler.setErrorPageBody();
	Result<std::string, bool>	res_1 = handler.getResponse().getHeader("Content-Length");
	std::string					content_len = res_1.getOk();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(content_len, expected_content_length);
}
