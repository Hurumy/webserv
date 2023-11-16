/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_errorpage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:39:24 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/07 12:20:31 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readErrorPage(Config &conf, std::string oneline) {
	std::vector<std::string> lines;
	std::stringstream ss;
	int num;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "error_page")
		ft::errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 3)
		ft::errorInInit("Too many argments in error_page directives ι(´Д｀υ)");

	if (isNumber(lines.at(1)) == false)
		ft::errorInInit(
			"Invalid HTTP statuscode in error_page directives "
			"▂▅▇█▓▒░(’ω’)░▒▓█▇▅▂");

	ss << lines.at(1);
	ss >> num;

	if (!(100 <= num && num < 600))
		ft::errorInInit(
			"Invalid HTTP statuscode in error_page directives "
			"▂▅▇█▓▒░(’ω’)░▒▓█▇▅▂");

	conf.addErrorPages(num, lines.at(2));
	Result<std::string, bool> res = conf.getErrorPages(num);

	// std::cout << MAGENTA "error_pages: " << num << ": " << res.getOk() <<
	// RESET << std::endl;

	return (0);
}

int l_readErrorPage(Location &loc, std::string oneline) {
	std::vector<std::string> lines;
	std::stringstream ss;
	int num;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "error_page")
		ft::errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 3)
		ft::errorInInit("Too many argments in error_page directives ι(´Д｀υ)");

	if (isNumber(lines.at(1)) == false)
		ft::errorInInit(
			"Invalid HTTP statuscode in error_page directives "
			"▂▅▇█▓▒░(’ω’)░▒▓█▇▅▂");

	ss << lines.at(1);
	ss >> num;

	if (!(100 <= num && num < 600))
		ft::errorInInit(
			"Invalid HTTP statuscode in error_page directives "
			"▂▅▇█▓▒░(’ω’)░▒▓█▇▅▂");

	loc.addErrorPages(num, lines.at(2));
	Result<std::string, bool> res = loc.getErrorPages(num);

	// std::cout << MAGENTA "error_pages: " << num << ": " << res.getOk() <<
	// RESET << std::endl;

	return (0);
}
