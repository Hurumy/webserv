/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_test.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 12:50:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/04 16:31:07 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <string>
#include "Response.hpp"

TEST(ResponseTest, versionTest) {
	Response response;
	std::string expected("HTTP/1.1");

	response.setVersion(expected);
	ASSERT_STREQ(response.getVersion().c_str(), expected.c_str());
}

TEST(ResponseTest, statusTest) {
	Response response;
	unsigned int expected(200);

	response.setStatus(expected);
	ASSERT_EQ(response.getStatus(), expected);
}

TEST(ResponseTest, statusMessageTest) {
	Response response;
	std::string expected("OK");

	response.setStatusMessage(expected);
	ASSERT_STREQ(response.getStatusMessage().c_str(), expected.c_str());
}

TEST(ResponseTest, bodyTest) {
	Response response;
	std::string expected("Hello, world.");

	response.setBody(expected);
	ASSERT_STREQ(response.getBody().c_str(), expected.c_str());
}

TEST(ResponseTest, headerTest01) {
	Response response;
	std::string expected("text/html;charset=UTF-8");

	response.addHeader("Content-Type", expected);
	ASSERT_EQ(response.getHeader("Content-Type").isOK(), true);
	ASSERT_EQ(response.getHeader("Content-Type").isError(), false);
	ASSERT_STREQ(response.getHeader("Content-Type").getOk().c_str(), expected.c_str());
}

TEST(ResponseTest, headerTest02) {
	Response response;
	std::string expected("text/html;charset=UTF-8");

	response.addHeader("Content-Type", expected);
	ASSERT_EQ(response.addHeader("Content-Type", expected), false);
}

TEST(ResponseTest, headerTest03) {
	Response response;

	response.addHeader("Content-Content", "Content");
	Result<std::string, bool> result = response.getHeader("Content-Type");
	ASSERT_EQ(result.isOK(), false);
	ASSERT_EQ(result.isError(), true);
	ASSERT_EQ(result.getError(), false);
}

TEST(ResponseTest, headerTest04) {
	Response response;

	Result<std::string, bool> result = response.getHeader("Content-Type");
	ASSERT_EQ(result.isOK(), false);
	ASSERT_EQ(result.isError(), true);
	ASSERT_EQ(result.getError(), false);
}

#define EXPECTED "\
HTTP/1.1 200 OK\r\n\
Content-Length: 14\r\n\
Content-Type: text/html;charset=UTF-8\r\n\
Date: Fri, 16 Mar 2018 17:36 27 GMT\r\n\
Server: test server\r\n\
\r\n\
Hello, world."

TEST(ResponseTest, linesTest) {
	Response response;
	std::string expected(EXPECTED);

	response.setVersion("HTTP/1.1");
	response.setStatus(200);
	response.setStatusMessage("OK");
	response.addHeader("Date", "Fri, 16 Mar 2018 17:36 27 GMT");
	response.addHeader("Server", "test server");
	response.addHeader("Content-Type", "text/html;charset=UTF-8");
	response.addHeader("Content-Length", "14");
	response.setBody("Hello, world.");
	ASSERT_STREQ(response.getLines().c_str(), expected.c_str());
}
