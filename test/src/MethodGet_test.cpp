/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet_test.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:25:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 11:24:15 by komatsud         ###   ########.fr       */
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

#define	CONF_FILE_PATH "testconfs/simple.conf"

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

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("./www/content/test.html");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(), expected_content_type);
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

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("./www/content/wtf.txt");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(), expected_content_type);
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

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("./www/content/mystery_file");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(handler.getResponse().getHeader("Content-Type").getOk(), expected_content_type);
}

TEST (MethodGetTest, getActTest_Error_NotFound)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(404);
	std::string					expected_string("Not Found");

	req.setVersion("HTTP/1.1");
	req.setMethod("GET");
	req.addHeader("Host", "_");
	req.setUrl("./www/content/there_is_no_file.txtxt");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
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


