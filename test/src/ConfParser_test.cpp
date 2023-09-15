/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:26:58 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 17:05:16 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "webserv.hpp"
#include "ConfParser.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

#define	CONF_FILE_PATH "testconfs/simple.conf"

TEST(ConfigParserTest, pPortTest)
{
	std::vector<Config>	tmp;
	int					expected_1(8660);
	int					expected_2(80);
	int					expected_3(8080);
	int					expected_4(80);
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getPort().at(0), expected_1);
	ASSERT_EQ(tmp.at(0).getPort().at(1), expected_2);
	ASSERT_EQ(tmp.at(1).getPort().at(0), expected_3);	
	ASSERT_EQ(tmp.at(1).getPort().at(1), expected_4);
}

TEST(ConfigParserTest, pIPTest)
{
	std::vector<Config>	tmp;
	std::string			expected_1("111.108.92.125");
	std::string			expected_2("::");
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getIpAddress().at(0), expected_1);
	ASSERT_EQ(tmp.at(0).getIpAddress().at(1), expected_2);
}

TEST(ConfigParserTest, pServerNameTest)
{
	std::vector<Config>	tmp;
	std::string			expected_1("_");
	std::string			expected_2("kawaii.test");
	std::string			expected_3("www.kawaii.test");
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getServerName().at(0), expected_1);
	ASSERT_EQ(tmp.at(1).getServerName().at(0), expected_2);
	ASSERT_EQ(tmp.at(1).getServerName().at(1), expected_3);
}

TEST(ConfigParserTest, pRootTest)
{
	std::vector<Config>	tmp;
	std::string			expected_1("_");
	std::string			expected_2("kawaii.test");
	std::string			expected_3("www.kawaii.test");
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getServerName().at(0), expected_1);
	ASSERT_EQ(tmp.at(1).getServerName().at(0), expected_2);
	ASSERT_EQ(tmp.at(1).getServerName().at(1), expected_3);
}

TEST(ConfigParserTest, pMaxBodySize)
{
	std::vector<Config>			tmp;
	unsigned long long			expected_1(5000000);
	unsigned long long			expected_2(10000);
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getMaxBodySize(), expected_1);
	ASSERT_EQ(tmp.at(1).getMaxBodySize(), expected_2);
}

TEST(ConfigParserTest, pErrorPagesTest)
{
	std::vector<Config>	tmp;
	int					key_1(404);
	int					key_2(500);
	int					key_3(400);
	int					key_4(501);
	std::string			expected_1("/404.html");
	std::string			expected_2("/500.html");
	std::string			expected_3("/error_pages/400.html");
	std::string			expected_4("/error_pages/501.html");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getErrorPages(key_1).getOk(), expected_1);
	ASSERT_EQ(tmp.at(0).getErrorPages(key_2).getOk(), expected_2);
	ASSERT_EQ(tmp.at(1).getErrorPages(key_3).getOk(), expected_3);
	ASSERT_EQ(tmp.at(1).getErrorPages(key_4).getOk(), expected_4);
}

TEST(ConfigParserTest, pRewriteTest)
{
	std::vector<Config>	tmp;
	std::string			key_1("test/test/what.html");
	std::string			key_2("/content/ok.html");
	std::string			expected_1("google.com");
	std::string			expected_2("yahoo.co.jp");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getRedirects(key_1).getOk(), expected_1);
	ASSERT_EQ(tmp.at(1).getRedirects(key_2).getOk(), expected_2);
}

TEST(ConfigParserTest, pReturnTest)
{
	std::vector<Config>	tmp;
	bool				expected_1(true);
	bool				expected_2(true);
	int					expected_3(440);
	std::string			expected_4("https://www.google.co.jp/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).isReturn(), expected_1);
	ASSERT_EQ(tmp.at(1).isReturn(), expected_2);
	ASSERT_EQ(tmp.at(0).getReturnStatus(), expected_3);
	ASSERT_EQ(tmp.at(1).getReturnUrl(), expected_4);
}

TEST(ConfigParserTest, pAutoIndexTest)
{
	std::vector<Config>	tmp;
	bool				expected_1(false);
	bool				expected_2(true);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getDirlist(), expected_1);
	ASSERT_EQ(tmp.at(1).getDirlist(), expected_2);
}
