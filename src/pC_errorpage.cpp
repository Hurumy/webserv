/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_errorpage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:39:24 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/14 16:43:17 by komatsud         ###   ########.fr       */
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
		errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 3)
		errorInInit("Too many argments in error_page directives ι(´Д｀υ)");

	if (isNumber(lines.at(1)) == false)
		errorInInit(
			"Invalid HTTP statuscode in error_page directives "
			"▂▅▇█▓▒░(’ω’)░▒▓█▇▅▂");

	ss << lines.at(1);
	ss >> num;

	if (!(100 <= num && num < 600))
		errorInInit(
			"Invalid HTTP statuscode in error_page directives "
			"▂▅▇█▓▒░(’ω’)░▒▓█▇▅▂");

	conf.addErrorPages(num, lines.at(2));
	Result<std::string, bool> res = conf.getErrorPages(num);

	// std::cout << MAGENTA "error_pages: " << num << ": " << res.getOk() <<
	// RESET << std::endl;

	return (0);
}
