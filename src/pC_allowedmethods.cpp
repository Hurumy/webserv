/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_allowedmethods.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:01:52 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/07 12:20:31 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readAllowedMethods(Config &conf, std::string oneline) {
	std::vector<std::string> lines;
	bool status;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "allowedMethods")
		ft::errorInInit("Unknown directive was detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() > 5)
		ft::errorInInit(
			"Too many elements in allowedMethods directives _(´ω`_)⌒)_ ))");

	for (size_t i = 1; i < lines.size(); i++) {
		if (lines.at(i) == "GET" || lines.at(i) == "POST" ||
			lines.at(i) == "DELETE") {
			status = conf.addReqMethod(lines.at(i), true);
			if (status == false) {
				ft::errorInInit("Too many allowedMethod is declare(´-ω-`)");
			}
		} else
			ft::errorInInit("Unsupported method is detected (*´ω｀*)");
	}

	return (0);
}

int l_readAllowedMethods(Location &loc, std::string oneline) {
	std::vector<std::string> lines;
	bool status;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "allowedMethods")
		ft::errorInInit("Unknown directive was detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() > 5)
		ft::errorInInit(
			"Too many elements in allowedMethods directives _(´ω`_)⌒)_ ))");

	for (size_t i = 1; i < lines.size(); i++) {
		if (lines.at(i) == "GET" || lines.at(i) == "POST" ||
			lines.at(i) == "DELETE") {
			status = loc.addReqMethod(lines.at(i), true);
			if (status == false) {
				ft::errorInInit("Too many allowedMethod is declare(´-ω-`)");
			}
		} else
			ft::errorInInit("Unsupported method is detected (*´ω｀*)");
	}

	return (0);
}
