/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_listen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:55:31 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/11 17:01:17 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int	readListen(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;
	unsigned long long							status;

	status = oneline.find(":", 0);
	lines = lineSpliter(oneline, " ");
	if (status == std::string::npos)
	{
		std::stringstream	ss;
		int					port;
		
		if (lines.size() != 2 && lines.size() != 3)
			return (-1);
		ss << lines.at(1);
		ss >> port;
		if (!(1 <= port && port <= 65535))
			return (-1);
		conf.setPort(port);
	}
	else
	{
		std::vector<std::string>	div;
		//int							port;
		//std::stringstream			ss;

		if (lines.size() != 2 && lines.size() != 3)
			return (-1);
		div = lineSpliter(lines.at(1), ":");
		
	}
	if (lines.size() == 3)
	{
		if (lines.at(2) != "default_server")
			return (-1);
		else
			return (1);
	}
	return (0);
}
