/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:26:58 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/18 17:28:52 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

#include <gtest/gtest.h>

#include "Address.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "webserv.hpp"

#define CONF_FILE_PATH "testconfs/simple.conf"
#define CONF_FILE_PATH_2 "testconfs/location_dir.conf"

TEST(ConfigParserTest, pAddressesTest)
{
	std::vector<Config> tmp;
	bool	expected(true);
	unsigned long	vecsize_1(2);
	int expected_1(8660);
	int expected_2(80);
	unsigned long	vecsize_2(2);
	int expected_3(8080);
	int expected_4(80);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	ASSERT_EQ(res.isOK(), expected);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getAddresses().size(), vecsize_1);
	ASSERT_EQ(tmp.at(0).getAddresses().at(0).getPort(), expected_1);
	ASSERT_EQ(tmp.at(0).getAddresses().at(1).getPort(), expected_2);
	ASSERT_EQ(tmp.at(1).getAddresses().size(), vecsize_2);
	ASSERT_EQ(tmp.at(1).getAddresses().at(0).getPort(), expected_3);
	ASSERT_EQ(tmp.at(1).getAddresses().at(1).getPort(), expected_4);
}

TEST(ConfigParserTest, pServerNameTest) {
	std::vector<Config> tmp;
	std::string expected_1("_");
	std::string expected_2("kawaii.test");
	std::string expected_3("www.kawaii.test");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getServerName().at(0), expected_1);
	ASSERT_EQ(tmp.at(1).getServerName().at(0), expected_2);
	ASSERT_EQ(tmp.at(1).getServerName().at(1), expected_3);
}

TEST(ConfigParserTest, pRootTest) {
	std::vector<Config> tmp;
	std::string expected_1("/usr/share/nginx/html");
	std::string expected_2("/content");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getRootDir(), expected_1);
	ASSERT_EQ(tmp.at(1).getRootDir(), expected_2);
}

TEST(ConfigParserTest, pMaxBodySize) {
	std::vector<Config> tmp;
	unsigned long long expected_1(5000000);
	unsigned long long expected_2(10000);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getMaxBodySize(), expected_1);
	ASSERT_EQ(tmp.at(1).getMaxBodySize(), expected_2);
}

TEST(ConfigParserTest, pErrorPagesTest) {
	std::vector<Config> tmp;
	int key_1(404);
	int key_2(500);
	int key_3(400);
	int key_4(501);
	std::string expected_1("/404.html");
	std::string expected_2("/500.html");
	std::string expected_3("/error_pages/400.html");
	std::string expected_4("/error_pages/501.html");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getErrorPages(key_1).getOk(), expected_1);
	ASSERT_EQ(tmp.at(0).getErrorPages(key_2).getOk(), expected_2);
	ASSERT_EQ(tmp.at(1).getErrorPages(key_3).getOk(), expected_3);
	ASSERT_EQ(tmp.at(1).getErrorPages(key_4).getOk(), expected_4);
}

TEST(ConfigParserTest, pRewriteTest) {
	std::vector<Config> tmp;
	std::string key_1("test/test/what.html");
	std::string key_2("/content/ok.html");
	std::string expected_1("google.com");
	std::string expected_2("yahoo.co.jp");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getRedirects(key_1).getOk(), expected_1);
	ASSERT_EQ(tmp.at(1).getRedirects(key_2).getOk(), expected_2);
}

TEST(ConfigParserTest, pReturnTest) {
	std::vector<Config> tmp;
	bool expected_1(true);
	bool expected_2(true);
	int expected_3(440);
	int expected_4(301);
	std::string expected_5("https://www.google.co.jp/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).isReturn(), expected_1);
	ASSERT_EQ(tmp.at(1).isReturn(), expected_2);
	ASSERT_EQ(tmp.at(0).getReturnStatus(), expected_3);
	ASSERT_EQ(tmp.at(1).getReturnStatus(), expected_4);
	ASSERT_EQ(tmp.at(1).getReturnUrl(), expected_5);
}

TEST(ConfigParserTest, pAutoIndexTest) {
	std::vector<Config> tmp;
	bool expected_1(false);
	bool expected_2(true);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getDirlist(), expected_1);
	ASSERT_EQ(tmp.at(1).getDirlist(), expected_2);
}

TEST(ConfigParserTest, pIndexTest) {
	std::vector<Config> tmp;
	std::string expected_1("index.html");
	std::string expected_2("index.php");
	std::string expected_3("index.test");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	// std::cout << res.isOK() << std::endl;
	tmp = res.getOk();
	// std::cout << tmp.at(0).getUploadPath() << std::endl;
	//  if (tmp.at(0).getIndex().empty() == true)
	//  	std::cout << "its empty!!" << std::endl;
	ASSERT_EQ(tmp.at(0).getIndex().at(0), expected_1);
	ASSERT_EQ(tmp.at(1).getIndex().at(0), expected_2);
	ASSERT_EQ(tmp.at(1).getIndex().at(1), expected_3);
}

TEST(ConfigParserTest, pUploadPathTest) {
	std::vector<Config> tmp;
	std::string expected_1("/content");
	std::string expected_2("Users/komatsud/webserv/html/www/content/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getUploadPath(), expected_1);
	ASSERT_EQ(tmp.at(1).getUploadPath(), expected_2);
}

TEST(ConfigParserTest, pAllowedMethodsTest) {
	std::vector<Config> tmp;
	bool expected_1(true);
	bool expected_2(true);
	bool expected_3(true);
	bool expected_4(false);
	bool expected_5(true);
	bool expected_6(true);
	bool expected_7(false);
	bool expected_8(false);

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

TEST(ConfigParserTest, parsingSomeLocationDirectivesTest) {
	std::vector<Config> tmp;
	bool expected_1(true);
	bool expected_2(true);
	bool expected_3(true);
	bool expected_4(false);
	bool expected_5(true);
	bool expected_6(true);
	bool expected_7(false);
	bool expected_8(false);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH_2);
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

TEST(ConfigParserTest, pCgiExtensionTest) {
	std::vector<Config> tmp;
	std::string expected_1("py");
	std::string expected_2("cgi");
	std::string expected_3("pl");
	bool ex_1(true);
	bool ex_2(true);
	bool ex_3(false);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getCgiExtension(expected_1).isOK(), ex_1);
	ASSERT_EQ(tmp.at(0).getCgiExtension(expected_2).isOK(), ex_2);
	ASSERT_EQ(tmp.at(0).getCgiExtension(expected_3).isOK(), ex_3);
}
