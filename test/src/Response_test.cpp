/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_test.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 12:50:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/03 17:03:44 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <string>
#include "Response.hpp"

TEST(ResponseTest, versionTest) {
	Response response;
	std::string expected("HTTP/1.1");

	response.setVersion(expected);
	ASSERT_EQ(response.getVersion().c_str(), expected);
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
	ASSERT_EQ(response.getStatusMessage().c_str(), expected);
}

TEST(ResponseTest, contentTypeTest) {
	Response response;
	std::string expected("text/plain");

	response.setContentType(expected);
	ASSERT_EQ(response.getContentType().c_str(), expected);
}

TEST(ResponseTest, contentLengthTest) {
	Response response;
	std::size_t expected(14);

	response.setContentLength(expected);
	ASSERT_EQ(response.getContentLength(), expected);
}

TEST(ResponseTest, bodyTest) {
	Response response;
	std::string expected("Hello, world.");

	response.setBody(expected);
	ASSERT_EQ(response.getBody().c_str(), expected);
}

TEST(ResponseTest, metaDataTest) {
	Response response;
	std::string expected("text/html;charset=UTF-8");

	response.addHeader("Content-Type", expected);
	ASSERT_EQ(response.getHeader("Content-Type").c_str(), expected);
}

#define EXPECTED "\
HTTP/1.1 200 OK\r\n\
Date: Fri, 16 Mar 2018 17:36 27 GMT\r\n\
Server: test server\r\n\
Content-Type: text/html;charset=UTF-8\r\n\
Content-Length: 14\r\n\
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
	response.setContentType("text/html;charset=UTF-8");
	response.setContentLength(14);
	response.setBody("Hello, world.");
	ASSERT_EQ(response.getLines().c_str(), expected);
}
