/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:51:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/15 22:50:07 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <gtest/gtest.h>

#include "CSocket.hpp"

TEST(RequestTest, setRequestLine01Test) {
	CSocket csocket(3);
	Request request;

	csocket.setData(
		"GET / HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: "
		"Mozilla/5.0\r\nAccept: text/html, */*\r\n\r\nHellow!");
	request.loadPayload(csocket);
	std::map<std::string, std::string> header = request.getAllHeader();
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
	CSocket csocket(3);
	Request request;

	csocket.setData("GET / HTTP/1.1\r\n");
	request.loadPayload(csocket);
	ASSERT_STREQ(request.getMethod().c_str(), "GET");
	ASSERT_STREQ(request.getUrl().c_str(), "/");
	ASSERT_STREQ(request.getVersion().c_str(), "HTTP/1.1");
}

TEST(RequestTest, setRequestLineError01Test) {
	CSocket csocket(3);
	Request request;

	csocket.setData("GET /");
	ASSERT_FALSE(request.loadPayload(csocket));
}

TEST(RequestTest, setRequestLineError02Test) {
	CSocket csocket(3);
	Request request;

	csocket.setData("");
	ASSERT_FALSE(request.loadPayload(csocket));
}

TEST(RequestTest, setRequestLineError03Test) {
	CSocket csocket(3);
	Request request;

	csocket.setData(
		"GET / HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: "
		"Mozilla/5.0\r\nAccept: text/html, */*");
	request.loadPayload(csocket);
	std::map<std::string, std::string> header = request.getAllHeader();
	for (std::map<std::string, std::string>::iterator iter = header.begin();
		 iter != header.end(); ++iter) {
		std::clog << "key: " << iter->first << std::endl;
		std::clog << "value: " << iter->second << std::endl;
	}
	ASSERT_STREQ(request.getMethod().c_str(), "GET");
	ASSERT_STREQ(request.getUrl().c_str(), "/");
	ASSERT_STREQ(request.getVersion().c_str(), "HTTP/1.1");
}
