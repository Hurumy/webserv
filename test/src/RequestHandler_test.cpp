/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:17:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/03 14:45:47 by komatsud         ###   ########.fr       */
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

#define CONF_FILE_PATH "testconfs/simple.conf"
#define CONF_FILE_WITH_ONE_LOC "testconfs/location_dir.conf"
#define CONF_FOR_ALIAS_TEST "testconfs/alias_test.conf"
#define CONF_FOR_CGI "testconfs/cgi_test.conf"
#define CONF_FOR_ROUTING_TEST "testconfs/routingtest.conf"

TEST(RequestHandlerTest, searchMatchHostTest) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	int expected(1);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(8080);

	// std::clog << RED << "Segmentation fault" << RESET << std::endl;
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
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(80);

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
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(8660);

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
	int default_portnum(8660);
	std::string default_hostname("_");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test:28282");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(28282);

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.getPortNumber(), default_portnum);
	ASSERT_EQ(handler.getHostname(), default_hostname);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_withWrongPort_2) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);
	int default_portnum(8660);
	std::string default_hostname("_");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "www.kawaii.test:9999");
	req.setUrl("/");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(9999);

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.getPortNumber(), default_portnum);
	ASSERT_EQ(handler.getHostname(), default_hostname);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_withWrongPort_3) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);
	int default_portnum(8660);
	std::string default_hostname("_");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_:9999");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(9999);

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.getPortNumber(), default_portnum);
	ASSERT_EQ(handler.getHostname(), default_hostname);
}

TEST(RequestHandlerTest, searchMatchHostTest_Error_WrongHost) {
	std::vector<Config> tmp;
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	Request req;
	bool expected(true);
	int default_portnum(8660);
	std::string default_hostname("_");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "wronghost.test.net");

	RequestHandler handler = RequestHandler(tmp, req);
	Result<int, bool> result_1 = handler.searchMatchHost();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.getPortNumber(), default_portnum);
	ASSERT_EQ(handler.getHostname(), default_hostname);
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
	Result<std::string, bool> res_1 =
		handler.getResponse().getHeader("Content-Length");
	std::string content_len = res_1.getOk();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(content_len, expected_content_length);
}

TEST(RequestHandlerTest, redirectionTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	unsigned int expected_status(440);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	// std::cout << handler.getResponse().getLines() << std::endl;
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
}

TEST(RequestHandlerTest, getCgiInfoTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected_status(true);
	std::string expected_path("/cgis/test.cgi");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "cgi.test");
	req.setLocalPort(8080);
	req.setLocalAddr("0.0.0.0");
	req.setUrl(expected_path);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();
	handler.isCgi();

	// std::cout << handler.getResponse().getLines() << std::endl;

	ASSERT_EQ(handler.isCgi().isOK(), expected_status);
	ASSERT_EQ(handler.isCgi().getOk(), "." + expected_path);
}

TEST(RequestHandlerTest, Error_ENOENT_getCgiInfoTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected_status(false);
	std::string expected_path("/cgis/noent.cgi");
	unsigned int expected_num(404);
	bool is_there_content_length(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "cgi.test");
	req.setLocalPort(8080);
	req.setLocalAddr("0.0.0.0");
	req.setUrl(expected_path);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();
	handler.isCgi();

	// std::cout << handler.getResponse().getLines() << std::endl;

	// isOKでErrorだった場合、どちらにせよすぐ打ち返す
	//(普通のリクエストだった時と同じオペレーション)
	ASSERT_EQ(handler.isCgi().isOK(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_num);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  is_there_content_length);
}

TEST(RequestHandlerTest, Error_EACCES_getCgiInfoTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected_status(false);
	std::string expected_path("/cgis/cannotexec.cgi");
	unsigned int expected_num(403);
	bool is_there_content_length(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "cgi.test");
	req.setLocalPort(8080);
	req.setLocalAddr("0.0.0.0");
	req.setUrl(expected_path);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();
	handler.isCgi();

	// std::cout << handler.getResponse().getLines() << std::endl;

	// isOKでErrorだった場合、どちらにせよすぐ打ち返す
	//(普通のリクエストだった時と同じオペレーション)
	ASSERT_EQ(handler.isCgi().isOK(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_num);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  is_there_content_length);
}

TEST(RequestHandlerTest, getHostnameTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	std::string expected_host("_");
	int expected_port(8660);
	bool iscgi(false);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/dummy/test");
	req.setLocalPort(expected_port);
	req.setLocalAddr(expected_host);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.isCgi().isOK(), iscgi);
	ASSERT_EQ(handler.getHostname(), expected_host);
	ASSERT_EQ(handler.getPortNumber(), expected_port);
}

TEST(RequestHandlerTest, setAliasTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FOR_ALIAS_TEST);
	std::vector<Config> tmp = res.getOk();
	Request req;
	std::string expected_host("_");
	int expected_port(8660);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_body("What the fuck....");
	bool expected_is_there_content_len(true);
	bool iscgi(false);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/test/conf/wtf.txt");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.isCgi().isOK(), iscgi);
	ASSERT_EQ(handler.getHostname(), expected_host);
	ASSERT_EQ(handler.getPortNumber(), expected_port);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(RequestHandlerTest, setConnectionHeaderTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	std::string expected_host("kawaii.test");
	int expected_port(8080);
	bool iscgi(false);
	bool isthereconnectionheader(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", expected_host);
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(8080);
	req.setUrl("/");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.isCgi().isOK(), iscgi);
	ASSERT_EQ(handler.getHostname(), expected_host);
	ASSERT_EQ(handler.getPortNumber(), expected_port);
	ASSERT_EQ(handler.getResponse().getHeader("Connection").isOK(),
			  isthereconnectionheader);
}

TEST(RequestHandlerTest, getCgiWithSomeQuerysTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FOR_CGI);
	std::vector<Config> tmp = res.getOk();
	Request req;
	std::string expected_host("cgi.test");
	int expected_port(80);
	bool iscgi(true);
	bool isthereconnectionheader(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", expected_host);
	req.setUrl("/cgis/test.py/test?test=query");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.isCgi().isOK(), iscgi);
	ASSERT_EQ(handler.getHostname(), expected_host);
	ASSERT_EQ(handler.getPortNumber(), expected_port);
	ASSERT_EQ(handler.getResponse().getHeader("Connection").isOK(),
			  isthereconnectionheader);
}

TEST(RequestHandlerTest, routingTest_1) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FOR_ROUTING_TEST);
	std::vector<Config> tmp = res.getOk();
	Request req;
	std::string expected_host("lastserver");
	int expected_port(4040);
	bool expected_is_there_content_len(true);
	bool iscgi(false);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", expected_host);
	req.setUrl("/test/conf/wtf.txt");
	req.setLocalAddr("1.1.1.1");
	req.setLocalPort(expected_port);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.isCgi().isOK(), iscgi);
	ASSERT_EQ(handler.getHostname(), expected_host);
	ASSERT_EQ(handler.getPortNumber(), expected_port);
	ASSERT_EQ(handler.getResponse().getHeader("Host").getOk(), expected_host);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(RequestHandlerTest, getQueryTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FOR_CGI);
	std::vector<Config> tmp = res.getOk();
	Request req;
	std::string expected_host("cgi.test");
	int expected_port(80);
	bool iscgi(true);
	bool isthereconnectionheader(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", expected_host);
	req.setUrl("/cgis/test.py/test?test=query");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	handler.checkRequiedHeader();
	handler.routeMethod();

	ASSERT_EQ(handler.isCgi().isOK(), iscgi);
	ASSERT_EQ(handler.getHostname(), expected_host);
	ASSERT_EQ(handler.getPortNumber(), expected_port);
	ASSERT_EQ(handler.getResponse().getHeader("Connection").isOK(),
			  isthereconnectionheader);
	ASSERT_EQ(handler.getQuery(), "test=query");
}

TEST(RequestHandlerTest, setCgiResponseTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	Response _res;
	bool expected(true);
	unsigned int expected_status(404);
	std::string expected_string("Not Found");
	std::string expected_body(
		"<H1>HTTP 404 Not Found</H1>\n\n<p>The requested URL was not found on "
		"this server.</p>");
	std::string expected_contentlen("83");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(8080);
	_res.setStatus(expected_status);
	_res.setStatusMessage(expected_string);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();

	handler.setCgiResponse(_res);

	// std::cout << handler.getResponse().getBody() << std::endl;
	// std::cout << handler.getResponse().getLines() << std::endl;

	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getHeader("Connection").isOK(), true);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").getOk(),
			  expected_contentlen);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
}

TEST(RequestHandlerTest, setErrorPageWithLocationTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	std::vector<Config> tmp = res.getOk();
	Request req;
	Response _res;
	bool expected(true);
	unsigned int expected_status(404);
	std::string expected_string("Not Found");
	std::string expected_body(
		"<H1>HTTP 404 Not Found</H1>\n\n<p>The requested URL was not found on "
		"this server.</p>");
	std::string expected_contentlen("83");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/test/conf/aokwndjskdnakjwnw.html");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(8660);
	_res.setStatus(expected_status);
	_res.setStatusMessage(expected_string);

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	handler.setCgiResponse(_res);

	// std::cout << handler.getResponse().getBody() << std::endl;
	// std::cout << handler.getResponse().getLines() << std::endl;

	ASSERT_EQ(result_1.isOK(), expected);
	ASSERT_EQ(handler.getResponse().getHeader("Connection").isOK(), true);
	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").getOk(),
			  expected_contentlen);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
}
