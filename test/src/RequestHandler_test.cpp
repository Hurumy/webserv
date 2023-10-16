/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:17:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 14:40:57 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include <gtest/gtest.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

#define	CONF_FILE_PATH "testconfs/simple.conf"
#define CONF_FILE_WITH_ONE_LOC "testconfs/location_dif.conf"

TEST(RequestHandlerTest, searchMatchHostTest) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	int expected(1);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.getOk(), expected);
}

TEST(RequestHandlerTest, searchMatchHostTest_withPort) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test:80");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);
}

TEST(RequestHandlerTest, searchMatchHostTest_withPort_2) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_:8660");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_withWrongPort) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(400);
	std::string expected_statusMessage("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test:28282");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isError(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_statusMessage);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_withWrongPort_2) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(400);
	std::string expected_statusMessage("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "www.kawaii.test:9999");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isError(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_statusMessage);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_withWrongPort_3) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(400);
	std::string expected_statusMessage("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_:9999");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isError(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_statusMessage);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_WrongHost) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(false);
	unsigned int expected_status(400);
	std::string expected_string("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "wronghost.test.net");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_NoHostHeaderInRequest) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(false);
	unsigned int expected_status(400);
	std::string expected_string("Bad Request");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST(RequestHandlerTest, checkRequiedHeaderTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);
}

TEST(RequestHandlerTest, checkRequiedHeaderTest_Error_HTTPVersion) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(false);
	unsigned int expected_status(505);
	std::string expected_string("HTTP Version Not Supported");

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

TEST(RequestHandlerTest, checkRequiedHeaderTest_Error_AllowedMethod) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(false);
	unsigned int expected_status(405);
	std::string expected_string("Method Not Allowed");

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

TEST(RequestHandlerTest, getResponseTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(false);
	unsigned int expected_status(405);
	std::string expected_string("Method Not Allowed");

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

TEST(RequestHandlerTest, setErrorPageBodyTest_Error_HTTPVersion) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(false);
	unsigned int expected_status(505);
	std::string expected_string("HTTP Version Not Supported");
	std::string expected_body(
		"<H1>HTTP 505 HTTP Version Not Supported</H1>\n\n<p>This webserv "
		"supports only HTTP/1.1...</p>");
	std::string expected_content_length("91");

	req.setVersion("HTTP/2.0");
	req.setMethod("GET");
	req.addHeader("Host", "_");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	// errorPageBody
	handler.setErrorPageBody();
	Result<std::string, bool> res_1 =
		handler.getResponse().getHeader("Content-Length");
	std::string content_len = res_1.getOk();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(content_len, expected_content_length);
}

TEST (RequestHandlerTest, redirectionTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	unsigned int				expected_status(440);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	//std::cout << handler.getResponse().getLines() << std::endl;
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
}

