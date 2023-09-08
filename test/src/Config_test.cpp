/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 10:12:05 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:03:19 by komatsud         ###   ########.fr       */
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


