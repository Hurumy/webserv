/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:26:58 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/14 18:02:32 by komatsud         ###   ########.fr       */
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

TEST(ConfigParserTest, listenTest)
{
	std::vector<Config>	tmp;
	int					expected_1(80);
	int					expected_2(80);
	int					expected_3(8080);
	
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	tmp = res.getOk();
	

}
