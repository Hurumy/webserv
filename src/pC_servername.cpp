/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_servername.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:43:12 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/14 16:43:55 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readServerName(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "server_name")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	for (size_t i = 1; i < lines.size(); i++) {
		conf.addServerName(lines.at(i));
		// std::cout << GREEN "server_name: " << conf.getServerName() << RESET
		// << std::endl;
	}

	return (0);
}
