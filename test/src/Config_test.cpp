/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:12:05 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 17:42:47 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <string>
#include "Config.hpp"

TEST(ConfigTest, ipaddressTest)
{
	Config config;
	std::vector<std::string>	res;
	std::string	expected("111.108.92.125");

	config.addIpAddress(expected);
	res = config.getIpAddress();
	ASSERT_EQ(res.at(0), expected);
}

TEST(ConfigTest, portTest)
{
	Config config;
	std::vector<int>	res;
	int	expected(8080);

	config.addPort(expected);
	res = config.getPort();
	ASSERT_EQ(res.at(0), expected);
}

TEST(ConfigTest, servernameTest)
{
	Config		config;
	std::vector<std::string>	res;
	std::string	expected("WEBSERV");

	config.addServerName(expected);
	res = config.getServerName();
	ASSERT_EQ(res.at(0), expected);
}

TEST(ConfigTest, rootDirTest)
{
	Config		config;
	std::string	expected("./content");

	config.setRootDir(expected);
	ASSERT_EQ(config.getRootDir(), expected);
}

TEST(ConfigTest, maxBodySizeTest)
{
	Config		config;
	unsigned long long	expected(50000);

	config.setMaxBodySize(expected);
	ASSERT_EQ(config.getMaxBodySize(), expected);
}

TEST(ConfigTest, errorPagesTest)
{
	Config		config;
	int	key(404);
	std::string val("./content/404.html");

	config.addErrorPages(key, val);
	Result<std::string, bool> res = config.getErrorPages(key);
	ASSERT_EQ(res.isOK(), true);
	std::string tmp = res.getOk();
	ASSERT_EQ(tmp, val);
}

TEST(ConfigTest, redirectsTest)
{
	Config		config;
	std::string	key("./index.html");
	std::string val("./content/404.html");

	config.addRedirects(key, val);
	Result<std::string, bool> res = config.getRedirects(key);
	ASSERT_EQ(res.isOK(), true);
	std::string tmp = res.getOk();
	ASSERT_EQ(res.getOk(), val);
}

TEST(ConfigTest, isreturnTest)
{
	Config	config;
	bool	expected(true);

	config.setIsReturn(expected);
	ASSERT_EQ(config.isReturn(), expected);
}

TEST(ConfigTest, returnstatusTest)
{
	Config	config;
	int		expected(300);

	config.setReturnStatus(expected);
	ASSERT_EQ(config.getReturnStatus(), expected);
}

TEST(ConfigTest, returnurlTest)
{
	Config	config;
	std::string	expected("google.com");

	config.setReturnUrl(expected);
	ASSERT_EQ(config.getReturnUrl(), expected);
}

TEST(ConfigTest, dirlistTest)
{
	Config		config;
	bool	expected(true);

	config.setDirlist(expected);
	ASSERT_EQ(config.getDirlist(), expected);
}

TEST(ConfigTest, indexTest)
{
	Config		config;
	std::string	expected("index.html");

	config.addIndex(expected);
	ASSERT_EQ(config.getIndex().at(0), expected);
}

TEST(ConfigTest, uploadPathTest)
{
	Config		config;
	std::string	expected("./usr/upload");

	config.setUploadPath(expected);
	ASSERT_EQ(config.getUploadPath(), expected);
}

TEST(ConfigTest, reqMethodTest)
{
	Config		config;
	std::string	key("GET");
	bool		val(true);

	config.addReqMethod(key, val);
	Result<std::string, bool> res = config.getReqMethod(key);
	ASSERT_EQ(res.isOK(), true);
	ASSERT_EQ(res.getOk(), "");
}

