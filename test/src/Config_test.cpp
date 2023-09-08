/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:12:05 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:13:04 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <string>
#include "Config.hpp"

TEST(ConfigTest, portTest)
{
	Config config;
	int	expected(8080);

	config.setPort(expected);
	ASSERT_EQ(config.getPort(), expected);
}

TEST(ConfigTest, servernameTest)
{
	Config		config;
	std::string	expected("WEBSERV");

	config.setServerName(expected);
	ASSERT_EQ(config.getServerName(), expected);
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

TEST(ConfigTest, dirlistTest)
{
	Config		config;
	bool	expected(true);

	config.setDirlist(expected);
	ASSERT_EQ(config.getDirlist(), expected);
}

TEST(ConfigTest, returnDirTest)
{
	Config		config;
	std::string	expected("./content");

	config.setReturnDir(expected);
	ASSERT_EQ(config.getReturnDir(), expected);
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

