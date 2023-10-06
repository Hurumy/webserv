/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:12:05 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 14:53:42 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <string>
#include "Config.hpp"
#include "Address.hpp"

TEST(ConfigTest, addressesTest)
{
	Config					config;
	Address					add;
	std::vector<Address>	res;
	std::string				expected("111.108.92.125");
	int						expected_port(8080);
	int						expected_ver(0);

	add.setIpAddress(expected);
	add.setPort(expected_port);
	add.setIpVers(expected_ver);
	config.addAddresses(add);
	res = config.getAddresses();
	ASSERT_EQ(res.at(0).getIpAddress(), expected);
	ASSERT_EQ(res.at(0).getPort(), expected_port);
	ASSERT_EQ(res.at(0).getIpVers(), expected_ver);
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

TEST(ConfigTest, returnbodyTest)
{
	Config	config;
	std::string	expected("that is a set text");

	config.setReturnBody(expected);
	ASSERT_EQ(config.getReturnBody(), expected);
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

