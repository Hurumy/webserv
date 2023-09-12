/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_listen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:55:31 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/12 09:05:14 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static int	checkProtocol(Config &conf, std::string oneline)
{
	unsigned long long			ipv6;

	ipv6 = oneline.find("[");
}

int	readListen(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;
	unsigned long long			status;



	status = oneline.find(":", 0);
	lines = lineSpliter(oneline, " ");
	if (lines.size() == 3  && lines.at(2) == "default_server")
		errorInInit("I'm literally so sorry but you cant set default_server in this webserv (ヾﾉ･ω･`)\n");
	if (status == std::string::npos)
	{
		std::stringstream	ss;
		int					port;
		
		if (lines.size() != 2)
			errorInInit("invalid form in http:server:listen directive ⊂(  っ☉ω☉)っ\n");
		ss << lines.at(1);
		ss >> port;
		if (!(1 <= port && port <= 65535))
			errorInInit("invalid port number in http:server:listen directive ⊂(  っ☉ω☉)っ\n");
		conf.setPort(port);
	}
	else
	{
		std::vector<std::string>	div;
		int							port;
		std::stringstream			ss;

		if (lines.size() != 2)
			errorInInit("invalid form in http:server:listen directive ⊂(  っ☉ω☉)っ\n");
		div = lineSpliter(lines.at(1), ":");
		
	}
	return (0);
}
