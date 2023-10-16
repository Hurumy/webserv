/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location_test.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 11:51:04 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 11:00:51 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Location.hpp"

TEST(LocationTest, getUriTest)
{
	Location		loc;
	std::string		expected("/");

	loc.setUri(expected);
	ASSERT_EQ(loc.getUri(), expected);
}

TEST(LocationTest, rootDirTest)
{
	Location		loc;
	std::string	expected("./content");

	loc.setRootDir(expected);
	ASSERT_EQ(loc.getRootDir(), expected);
}

TEST(LocationTest, maxBodySizeTest)
{
	Location		loc;
	unsigned long long	expected(50000);

	loc.setMaxBodySize(expected);
	ASSERT_EQ(loc.getMaxBodySize(), expected);
}

TEST(LocationTest, errorPagesTest)
{
	Location		loc;
	int	key(404);
	std::string val("./content/404.html");

	loc.addErrorPages(key, val);
	Result<std::string, bool> res = loc.getErrorPages(key);
	ASSERT_EQ(res.isOK(), true);
	std::string tmp = res.getOk();
	ASSERT_EQ(tmp, val);
}

TEST(LocationTest, redirectsTest)
{
	Location		loc;
	std::string	key("./index.html");
	std::string val("./content/404.html");

	loc.addRedirects(key, val);
	Result<std::string, bool> res = loc.getRedirects(key);
	ASSERT_EQ(res.isOK(), true);
	std::string tmp = res.getOk();
	ASSERT_EQ(res.getOk(), val);
}

TEST(LocationTest, isreturnTest)
{
	Location	loc;
	bool	expected(true);

	loc.setIsReturn(expected);
	ASSERT_EQ(loc.isReturn(), expected);
}

TEST(LocationTest, returnstatusTest)
{
	Location	loc;
	int		expected(300);

	loc.setReturnStatus(expected);
	ASSERT_EQ(loc.getReturnStatus(), expected);
}

TEST(LocationTest, returnurlTest)
{
	Location	loc;
	std::string	expected("google.com");

	loc.setReturnUrl(expected);
	ASSERT_EQ(loc.getReturnUrl(), expected);
}

TEST(LocationTest, returnbodyTest)
{
	Location	loc;
	std::string	expected("this is a set text");

	loc.setReturnBody(expected);
	ASSERT_EQ(loc.getReturnBody(), expected);
}

TEST(LocationTest, dirlistTest)
{
	Location		loc;
	bool	expected(true);

	loc.setDirlist(expected);
	ASSERT_EQ(loc.getDirlist(), expected);
}

TEST(LocationTest, indexTest)
{
	Location		loc;
	std::string	expected("index.html");

	loc.addIndex(expected);
	ASSERT_EQ(loc.getIndex().at(0), expected);
}

TEST(LocationTest, uploadPathTest)
{
	Location		loc;
	std::string	expected("./usr/upload");

	loc.setUploadPath(expected);
	ASSERT_EQ(loc.getUploadPath(), expected);
}

TEST(LocationTest, reqMethodTest)
{
	Location		loc;
	std::string	key("GET");
	bool		val(true);

	loc.addReqMethod(key, val);
	Result<std::string, bool> res = loc.getReqMethod(key);
	ASSERT_EQ(res.isOK(), true);
	ASSERT_EQ(res.getOk(), "");
}

TEST(LocationTest, cgiExtensionTest)
{
	Location	location;
	std::string	ext_1("py");
	std::string ext_2("cgi");
	std::string	ext_3("pl");
	bool		expected(true);
	bool		expected_2(true);
	bool		expected_3(false);

	location.addCgiExtension(ext_1);
	location.addCgiExtension(ext_2);
	ASSERT_EQ(location.getCgiExtension(ext_1).isOK(), expected);
	ASSERT_EQ(location.getCgiExtension(ext_2).isOK(), expected_2);
	ASSERT_EQ(location.getCgiExtension(ext_3).isOK(), expected_3);
}





