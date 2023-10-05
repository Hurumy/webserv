/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationParser_test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:27:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/05 12:15:08 by komatsud         ###   ########.fr       */
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

#define	CONF_FILE_WITH_NO_LOC "testconfs/simple.conf"
#define CONF_FILE_WITH_ONE_LOC "testconfs/location_dir.conf"

TEST(LocationParserTest, pUriTest)
{
	std::vector<Config>	tmp;
	bool				expected_1(true);
	std::string			expected_path("/test/conf/");

	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(expected_path).isOK(), expected_1);
	ASSERT_EQ(tmp.at(0).getLocations(expected_path).getOk().getUri(), expected_path);
}

TEST(LocationParserTest, pRootTest)
{
	std::vector<Config>	tmp;
	bool				getstatus(true);
	std::string			location_path("/test/conf/");
	std::string			expected_1("/usr/share/nginx/html");
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_WITH_ONE_LOC);
	tmp = res.getOk();
	ASSERT_EQ(tmp.at(0).getLocations(location_path).isOK(), getstatus);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getUri(), location_path);
	ASSERT_EQ(tmp.at(0).getLocations(location_path).getOk().getRootDir(), expected_1);
}


