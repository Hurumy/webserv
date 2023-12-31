/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:51:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/05 14:42:42 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <gtest/gtest.h>

#include "CSocket.hpp"
#include "sComp.hpp"

TEST(RequestTest, setRequestLine01Test) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	Request request;

	csocket.setData(
		"GET / HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: "
		"Mozilla/5.0\r\nAccept: text/html, */*\r\n\r\nHellow!");
	request.loadPayload(csocket);
	std::map<std::string, std::string, sComp> header = request.getAllHeader();
	for (std::map<std::string, std::string>::iterator iter = header.begin();
		 iter != header.end(); ++iter) {
		std::clog << "key: " << iter->first << std::endl;
		std::clog << "value: " << iter->second << std::endl;
	}
	ASSERT_STREQ(request.getMethod().c_str(), "GET");
	ASSERT_STREQ(request.getUrl().c_str(), "/");
	ASSERT_STREQ(request.getVersion().c_str(), "HTTP/1.1");
}

TEST(RequestTest, setRequestLine02Test) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	Request request;

	csocket.setData("GET / HTTP/1.1\r\n");
	request.loadPayload(csocket);
	ASSERT_STREQ(request.getMethod().c_str(), "GET");
	ASSERT_STREQ(request.getUrl().c_str(), "/");
	ASSERT_STREQ(request.getVersion().c_str(), "HTTP/1.1");
}

TEST(RequestTest, setRequestLineError01Test) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	Request request;

	csocket.setData("GET /");
	ASSERT_FALSE(request.loadPayload(csocket));
}

TEST(RequestTest, setRequestLineError02Test) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	Request request;

	csocket.setData("");
	ASSERT_FALSE(request.loadPayload(csocket));
}

TEST(RequestTest, setRequestLineError03Test) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	Request request;

	csocket.setData(
		"GET / HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: "
		"Mozilla/5.0\r\nAccept: text/html, */*");
	request.loadPayload(csocket);
	std::map<std::string, std::string, sComp> header = request.getAllHeader();
	for (std::map<std::string, std::string>::iterator iter = header.begin();
		 iter != header.end(); ++iter) {
		std::clog << "key: " << iter->first << std::endl;
		std::clog << "value: " << iter->second << std::endl;
	}
	ASSERT_STREQ(request.getMethod().c_str(), "GET");
	ASSERT_STREQ(request.getUrl().c_str(), "/");
	ASSERT_STREQ(request.getVersion().c_str(), "HTTP/1.1");
}

TEST(RequestTest, getHeaderIgnoringCasesTest) {
	Request request;
	std::string contentlen("50011");
	std::string server("VeryGoodServer");
	std::string con("keep-alive");

	request.addHeader("Content-Length", contentlen);
	request.addHeader("Server", server);
	request.addHeader("CONNECT", con);

	ASSERT_EQ(request.getHeader("content-length").getOk(), contentlen);
	ASSERT_EQ(request.getHeader("server").getOk(), server);
	ASSERT_EQ(request.getHeader("connect").getOk(), con);
}

TEST(RequestTest, setHeaderTest) {
	Request request;
	std::string contentlen("50011");
	std::string server("VeryGoodServer");
	std::string con("keep-alive");

	request.addHeader("Content-Length", contentlen);
	request.addHeader("Server", server);
	request.addHeader("CONNECT", "close");
	request.setHeader("Connect", con);

	ASSERT_EQ(request.getHeader("Content-Length").getOk(), contentlen);
	ASSERT_EQ(request.getHeader("Server").getOk(), server);
	ASSERT_EQ(request.getHeader("Connect").getOk(), con);
}
