/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port_test.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:23:46 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:55:59 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Port.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include <string>

TEST(PortTest, getConfTest)
{
	Port	port;
	Config	conf_a;
	Config	conf_b;
	int	a_port(8080);
	int	b_port(90);
	std::string	a_name("SERVER_A");
	std::string	b_name("SERVER_B");

	conf_a.setServerName(a_name);
	conf_b.setServerName(b_name);
	port.addConf(a_port, conf_a);
	port.addConf(b_port, conf_b);
	
	Result<Config, bool> res = port.getConf(a_port);
	ASSERT_EQ(res.isOK(), true);
	ASSERT_EQ(res.getOk().getServerName(), a_name);
	Result<Config, bool> res2 = port.getConf(b_port);
	ASSERT_EQ(res2.isOK(), true);
	ASSERT_EQ(res2.getOk().getServerName(), b_name);
}

TEST(PortTest, getHostPortTest)
{
	Port	port;
	Config	conf_a;
	Config	conf_b;
	int	a_port(8080);
	int	b_port(90);
	std::string	a_name("SERVER_A");
	std::string	b_name("SERVER_B");

	conf_a.setServerName(a_name);
	conf_b.setServerName(b_name);
	port.addConf(a_port, conf_a);
	port.addConf(b_port, conf_b);
	
	port.setHostPort(a_port);

	int	host = port.getHostPort();
	Result<Config, bool> res = port.getConf(host);
	ASSERT_EQ(res.isOK(), true);
	ASSERT_EQ(res.getOk().getServerName(), a_name);
}

TEST(PortTest, getNumofPortsTest)
{
	Port	port;
	Config	conf_a;
	Config	conf_b;
	int	a_port(8080);
	int	b_port(90);
	size_t	ports = 0;

	ASSERT_EQ(port.getNumofPorts(), ports);
	
	port.addConf(a_port, conf_a);
	ports ++;

	ASSERT_EQ(port.getNumofPorts(), ports);

	port.addConf(b_port, conf_b);
	ports ++;

	ASSERT_EQ(port.getNumofPorts(), ports);	
}
