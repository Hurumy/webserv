/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_autoindex.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:47:31 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/14 14:21:14 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readAutoindex(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "autoindex")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit("Invalid number of elements _(´ω`_)⌒)_ ))");

	// std::cout << lines.at(1) << std::endl;

	if (lines.at(1) == "on")
		conf.setDirlist(true);
	else if (lines.at(1) == "off")
		conf.setDirlist(false);
	else
		errorInInit(
			"Invalid setting is detected in autoindex directive (¦3[___]");

	// std::cout << GREEN "autoindex is now: " << conf.getDirlist() << RESET <<
	// std::endl;

	return (0);
}

int l_readAutoindex(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "autoindex")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit("Invalid number of elements _(´ω`_)⌒)_ ))");

	if (lines.at(1) == "on")
		loc.setDirlist(true);
	else if (lines.at(1) == "off")
		loc.setDirlist(false);
	else
		errorInInit(
			"Invalid setting is detected in autoindex directive (¦3[___]");

	// std::cout << GREEN "autoindex is now: " << conf.getDirlist() << RESET <<
	// std::endl;

	return (0);
}
