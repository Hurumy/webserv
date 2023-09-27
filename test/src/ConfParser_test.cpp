/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:26:58 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/27 17:01:06 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "webserv.hpp"
#include "ConfParser.hpp"
#include "Address.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

#define	CONF_FILE_PATH "testconfs/simple.conf"

TEST(ConfigParserTest, pAddressesTest)
{
	std::vector<Config>	tmp;
	int					expected_1(8660);
	int					expected_2(80);
	int					expected_3(8080);
	int					expected_4(80);
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getAddresses().at(0).getPort(), expected_1);
	ASSERT_EQ(tmp.at(0).getAddresses().at(1).getPort(), expected_2);
	ASSERT_EQ(tmp.at(1).getAddresses().at(0).getPort(), expected_3);
	ASSERT_EQ(tmp.at(1).getAddresses().at(1).getPort(), expected_4);
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
	std::string			expected_1("/usr/share/nginx/html");
	std::string			expected_2("/content");
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getRootDir(), expected_1);
	ASSERT_EQ(tmp.at(1).getRootDir(), expected_2);
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

TEST(ConfigParserTest, pIndexTest)
{
	std::vector<Config>	tmp;
	std::string			expected_1("index.html");
	std::string			expected_2("index.php");
	std::string			expected_3("index.test");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getIndex().at(0), expected_1);
	//ASSERT_EQ(tmp.at(1).getIndex().at(0), expected_2);
	//ASSERT_EQ(tmp.at(1).getIndex().at(1), expected_3);
}

TEST(ConfigParserTest, pUploadPathTest)
{
	std::vector<Config>	tmp;
	std::string			expected_1("/content");
	std::string			expected_2("Users/komatsud/webserv/html/www/content/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getUploadPath(), expected_1);
	ASSERT_EQ(tmp.at(1).getUploadPath(), expected_2);
}

TEST(ConfigParserTest, pAllowedMethodsTest)
{
	std::vector<Config>	tmp;
	bool			expected_1(true);
	bool			expected_2(true);
	bool			expected_3(true);
	bool			expected_4(false);
	bool			expected_5(true);
	bool			expected_6(true);
	bool			expected_7(false);
	bool			expected_8(false);	

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getReqMethod("GET").isOK(), expected_1);
	ASSERT_EQ(tmp.at(0).getReqMethod("POST").isOK(), expected_2);
	ASSERT_EQ(tmp.at(0).getReqMethod("DELETE").isOK(), expected_3);
	ASSERT_EQ(tmp.at(0).getReqMethod("PUT").isOK(), expected_4);
	ASSERT_EQ(tmp.at(1).getReqMethod("GET").isOK(), expected_5);
	ASSERT_EQ(tmp.at(1).getReqMethod("POST").isOK(), expected_6);
	ASSERT_EQ(tmp.at(1).getReqMethod("DELETE").isOK(), expected_7);
	ASSERT_EQ(tmp.at(1).getReqMethod("PUT").isOK(), expected_8);
}
