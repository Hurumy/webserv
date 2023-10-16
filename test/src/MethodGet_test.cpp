/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet_test.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:25:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 13:42:14 by komatsud         ###   ########.fr       */
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
#include "AMethod.hpp"
#include "MethodGet.hpp"

#define	CONF_FILE_PATH "testconfs/method_get.conf"

TEST (MethodGetTest, getHtmlTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(200);
	std::string					expected_string("OK");
	std::string					expected_body("<p>Its VRry htMl fiLe fOr Test!!</p>");
	std::string					expected_content_length("36");
	std::string					expected_content_type("text/html");
	bool						expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("/test.html");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	std::cout << handler.getResponse().getStatus() << std::endl;
	std::cout << handler.getResponse().getStatusMessage() << std::endl;

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(), expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(), expected_is_there_content_len);
}

TEST (MethodGetTest, getTxtTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(200);
	std::string					expected_string("OK");
	std::string					expected_body("What the fuck....");
	std::string					expected_content_length("17");
	std::string					expected_content_type("text/plain");
	bool						expected_is_there_content_len(true);

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
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(), expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(), expected_is_there_content_len);
}

TEST (MethodGetTest, getMysteryFileTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(200);
	std::string					expected_string("OK");
	std::string					expected_body("???\n01010101");
	std::string					expected_content_length("14");
	std::string					expected_content_type("text/plain");
	bool						expected_is_there_content_len(true);

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
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(), expected_content_type);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(), expected_is_there_content_len);
}

TEST (MethodGetTest, getActTest_Error_NotFound)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(404);
	std::string					expected_string("Not Found");
	bool						expected_is_there_content_len(true);

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
	ASSERT_EQ(handler.getResponse().getHeader("Content-Length").isOK(), expected_is_there_content_len);
}

// TEST (MethodGetTest, getActTest_Error_PermDenied)
// {
// 	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
// 	std::vector<Config>			tmp = res.getOk();
// 	Request						req;
// 	bool						expected(true);
// 	unsigned int				expected_status(403);
// 	std::string					expected_string("Forbidden");

// 	req.setVersion("HTTP/1.1");
// 	req.setMethod("GET");
// 	req.addHeader("Host", "_");
// 	req.setUrl("./www/content/NoPermToRead.txt");

// 	RequestHandler handler = RequestHandler(tmp, req);
// 	handler.searchMatchHost();
// 	Result<int, bool> result_1 = handler.checkRequiedHeader();
// 	ASSERT_EQ(result_1.isOK(), expected);

// 	handler.routeMethod();

// 	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
// 	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
// }



