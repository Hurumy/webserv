/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:27:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 10:37:13 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Address.hpp"
#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "webserv.hpp"

#define CONF_FILE_WITH_NO_LOC "testconfs/simple.conf"
#define CONF_FILE_WITH_ONE_LOC "testconfs/location_dir.conf"

TEST(LocationParserTest, pUriTest) {
	std::vector<Config> tmp;
	bool expected_1(true);
	std::string expected_path("/test/conf/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(expected_path).isOK(), expected_1);
	ASSERT_EQ(tmp.at(0).getLocations(expected_path).getOk().getUri(),
			  expected_path);
}

TEST(LocationParserTest, pRootTest) {
	std::vector<Config> tmp;
	bool getstatus(true);
	std::string location_path("/test/conf/");
	std::string expected_1("/usr/share/nginx/html");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).isOK(), getstatus);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getUri(),
			  location_path);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getRootDir(),
			  expected_1);
}

TEST(LocationParserTest, pErrorPagesTest) {
	std::vector<Config> tmp;
	bool getstatus(true);
	std::string location_path("/test/conf/");
	std::string expected_1("/404.html");
	std::string expected_2("/500.html");
	std::string expected_3("error_pages/505.html");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).isOK(), getstatus);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getUri(),
			  location_path);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getErrorPages(404)
				  .getOk(),
			  expected_1);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getErrorPages(500)
				  .getOk(),
			  expected_2);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getErrorPages(505)
				  .getOk(),
			  expected_3);
}

TEST(LocationParserTest, pReturnTest) {
	std::vector<Config> tmp;
	bool getstatus(true);
	std::string location_path("/test/conf/");
	bool expected_1(true);
	int expected_2(301);
	std::string expected_3("https://www.google.com");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).isOK(), getstatus);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getUri(),
			  location_path);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().isReturn(),
			  expected_1);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getReturnStatus(),
			  expected_2);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getReturnUrl(),
			  expected_3);
}

TEST(LocationParserTest, pRewriteTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	std::string key_1("/content/ok.html");
	std::string expected_1("test.co.jp");
	std::string key_2("/test/index.html");
	std::string expected_2("google.com");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getRedirects(key_1)
				  .getOk(),
			  expected_1);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getRedirects(key_2)
				  .getOk(),
			  expected_2);
}

TEST(LocationParserTest, pAutoIndexTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	bool expected_1(true);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getDirlist(),
			  expected_1);
}

TEST(LocationParserTest, pMaxBodySize) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	unsigned long long expected_1(5000000);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getMaxBodySize(),
			  expected_1);
}

TEST(LocationParserTest, pIndexTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	std::string expected("index.php");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getIndex().at(0),
			  expected);
}

TEST(LocationParserTest, pUploadPathTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	std::string expected("Users/komatsud/webserv/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getUploadPath(),
			  expected);
}

TEST(LocationParserTest, pAllowedMethodsTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	bool expected_1(true);
	bool expected_2(true);
	bool expected_3(false);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getReqMethod("GET")
				  .isOK(),
			  expected_1);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getReqMethod("POST")
				  .isOK(),
			  expected_2);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getReqMethod("DELETE")
				  .isOK(),
			  expected_3);
}

TEST(LocationParserTest, pCgiExtensionTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	std::string expected_1("py");
	std::string expected_2("test");
	std::string expected_3("cgi");
	bool ex_1(true);
	bool ex_2(true);
	bool ex_3(false);

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getCgiExtension(expected_1)
				  .isOK(),
			  ex_1);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getCgiExtension(expected_2)
				  .isOK(),
			  ex_2);
	ASSERT_EQ(tmp.at(0)
				  .getLocations(location_path)
				  .getOk()
				  .getCgiExtension(expected_3)
				  .isOK(),
			  ex_3);
}

TEST(LocationParserTest, pAliasTest) {
	std::vector<Config> tmp;
	std::string location_path("/test/conf/");
	std::string expected_1("/mint/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();

	// std::cout << "num of confs: " << tmp.size() << std::endl;
	// std::cout << tmp.at(2).getAddresses().size() << std::endl;
	// std::cout << tmp.at(2).getAddresses().at(0).getIpAddress() << std::endl;
	// std::cout << tmp.at(2).getAddresses().at(0).getPort() << std::endl;

	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getAlias(),
			  expected_1);
}
