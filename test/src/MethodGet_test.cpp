/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet_test.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:25:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/30 15:49:00 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodGet.hpp"

#include <gtest/gtest.h>

#include "AMethod.hpp"
#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

#define CONF_FILE_PATH "testconfs/method_get.conf"
#define CONF_FILE_DIRLIST_TEST "testconfs/dirlisting_test.conf"
#define CONF_FILE_WITH_ONE_LOC "testconfs/location_dir.conf"

TEST(MethodGetTest, getHtmlTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_body("<p>Its VRry htMl fiLe fOr Test!!</p>");
	std::string expected_content_length("36");
	std::string expected_content_type("text/html");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/test.html");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	// std::cout << handler.getResponse().getStatus() << std::endl;
	// std::cout << handler.getResponse().getStatusMessage() << std::endl;

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(),
			  expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getTxtTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_body("What the fuck....");
	std::string expected_content_length("17");
	std::string expected_content_type("text/plain");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/wtf.txt");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(),
			  expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getMysteryFileTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_body("???\n01010101");
	std::string expected_content_length("14");
	std::string expected_content_type("text/plain");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/mystery_file");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(),
			  expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getActTest_Error_NotFound) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(404);
	std::string expected_string("Not Found");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/there_is_no_file.txtxt");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getActTest_getDirlistTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_DIRLIST_TEST);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	bool expected_stat(false);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "kawaii.test");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(8080);
	req.setUrl("/www/content/");

	// routing
	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();

	// request style check
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	// acting
	handler.routeMethod();

	// cgi test
	Result<std::string, bool> cgi_res = handler.isCgi();
	ASSERT_EQ(cgi_res.isOK(), expected_stat);

	// std::cout << "handler: " << handler.getResponse().getBody() << std::endl;

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getActTest_getIndexTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	bool expected_stat(false);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	bool expected_is_there_content_len(true);
	std::string expected_hostname("wtf.net");
	int expected_portnum(25565);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", expected_hostname);
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(expected_portnum);
	req.setUrl("/");

	// routing
	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();

	// request style check
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	// acting
	handler.routeMethod();

	// cgi test
	Result<std::string, bool> cgi_res = handler.isCgi();
	ASSERT_EQ(cgi_res.isOK(), expected_stat);

	// std::cout << "handler: " << handler.getResponse().getBody() << std::endl;
	// std::cout << handler.getHostname() << std::endl;
	// std::cout << handler.getPortNumber() << std::endl;

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getHostname(), expected_hostname);
	ASSERT_EQ(handler.getPortNumber(), expected_portnum);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getActTest_getIndexTest_FromLocation) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	bool expected_stat(false);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_body("What the fuck....");
	bool expected_is_there_content_len(true);
	std::string expected_hostname("wtf.net");
	int expected_portnum(25565);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "wtf.net");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(expected_portnum);
	req.setUrl("/");

	// routing
	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();

	// request style check
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	// acting
	handler.routeMethod();

	// cgi test
	Result<std::string, bool> cgi_res = handler.isCgi();
	ASSERT_EQ(cgi_res.isOK(), expected_stat);

	// std::cout << "response body: " << handler.getResponse().getBody() <<
	// std::endl; std::cout << handler.getHostname() << std::endl; std::cout <<
	// handler.getPortNumber() << std::endl;

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getHostname(), expected_hostname);
	ASSERT_EQ(handler.getPortNumber(), expected_portnum);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getQueryTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	bool expected_stat(false);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_body("What the fuck....");
	bool expected_is_there_content_len(true);
	std::string expected_hostname("wtf.net");
	std::string expected_query("test=query");
	int expected_portnum(25565);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "wtf.net");
	req.setLocalAddr("0.0.0.0");
	req.setLocalPort(expected_portnum);
	req.setUrl("/?" + expected_query);

	// routing
	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();

	// request style check
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	// acting
	handler.routeMethod();

	// cgi test
	Result<std::string, bool> cgi_res = handler.isCgi();
	ASSERT_EQ(cgi_res.isOK(), expected_stat);

	// std::cout << "response body: " << handler.getResponse().getBody() <<
	// std::endl; std::cout << handler.getHostname() << std::endl; std::cout <<
	// handler.getPortNumber() << std::endl;

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getHostname(), expected_hostname);
	ASSERT_EQ(handler.getPortNumber(), expected_portnum);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}

TEST(MethodGetTest, getBinaryTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(200);
	std::string expected_string("OK");
	std::string expected_content_length("50011");
	std::string expected_content_type("image/jpeg");
	bool expected_is_there_content_len(true);
	std::string expected_body;

	// expected_bodyを作る
	std::ifstream ifs("./www/content/icon.jpg", std::ios_base::binary);
	ifs.seekg(0, std::ios::end);
	long long int size = ifs.tellg();
	ifs.seekg(0);
	char buf[size + 1];
	ifs.read(buf, size);
	buf[size] = '\0';
	expected_body.assign(buf, size);
	ifs.close();

	// std::cout << expected_body << std::endl;

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/icon.jpg");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(),
			  expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(),
			  expected_is_there_content_len);
}
