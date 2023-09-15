/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:26:58 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 10:50:11 by komatsud         ###   ########.fr       */
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


