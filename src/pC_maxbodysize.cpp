/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_maxbodysize.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:52:58 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/07 12:20:31 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static bool isNumber_andUnit(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (!('0' <= str.c_str()[i] && str.c_str()[i] <= '9') &&
			str.c_str()[i] != 'm' && str.c_str()[i] != 'k')
			return (false);
	}
	return (true);
}

int readMaxBodySize(Config &conf, std::string oneline) {
	std::vector<std::string> lines;
	std::stringstream ss;
	unsigned long long size;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "client_max_body_size")
		ft::errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		ft::errorInInit("Invalid form detected in return directives ⊂('ω`⊂ 三");

	if (isNumber_andUnit(lines.at(1)) == false)
		ft::errorInInit(
			"Invalid expression detected in client_max_body_size directive "
			"(´っω-).｡oO");

	replaceStr(lines.at(1), "k", "000");
	replaceStr(lines.at(1), "m", "000000");

	ss << lines.at(1);
	ss >> size;

	conf.setMaxBodySize(size);
	return (0);
}

int l_readMaxBodySize(Location &loc, std::string oneline) {
	std::vector<std::string> lines;
	std::stringstream ss;
	unsigned long long size;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "client_max_body_size")
		ft::errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		ft::errorInInit("Invalid form detected in return directives ⊂('ω`⊂ 三");

	if (isNumber_andUnit(lines.at(1)) == false)
		ft::errorInInit(
			"Invalid expression detected in client_max_body_size directive "
			"(´っω-).｡oO");

	replaceStr(lines.at(1), "k", "000");
	replaceStr(lines.at(1), "m", "000000");

	ss << lines.at(1);
	ss >> size;

	loc.setMaxBodySize(size);
	return (0);
}
