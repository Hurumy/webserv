/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodDelete_test.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:23:41 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 11:24:21 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodDelete.hpp"

#include <gtest/gtest.h>

#include "ConfParser.hpp"
#include "RequestHandler.hpp"

#define CONF_FILE_PATH "testconfs/method_delete.conf"

TEST(MethodDeleteTest, deletePostedFileTest) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(204);
	std::string expected_string("No Content");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("DELETE");
	req.addHeader("Host", "_");
	req.setUrl("/post/0");

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

TEST(MethodDeleteTest, deletePostedFile2Test) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(204);
	std::string expected_string("No Content");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("DELETE");
	req.addHeader("Host", "_");
	req.setUrl("/post/1");

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

TEST(MethodDeleteTest, deletePostedFileTest_Error_NotFound) {
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config> tmp = res.getOk();
	Request req;
	bool expected(true);
	unsigned int expected_status(404);
	std::string expected_string("Not Found");
	bool expected_is_there_content_len(true);

	req.setVersion("HTTP/1.1");
	req.setMethod("DELETE");
	req.addHeader("Host", "_");
	req.setUrl("/dummy/thereisnofile.html");

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
