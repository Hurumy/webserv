/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_rewrite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:23:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/14 14:20:59 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readRewrite(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "rewrite")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 3)
		errorInInit("Invalid number of elements _(´ω`_)⌒)_ ))");

	conf.addRedirects(lines.at(1), lines.at(2));

	Result<std::string, bool> res = conf.getRedirects(lines.at(1));

	return (0);
}

int l_readRewrite(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "rewrite")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 3)
		errorInInit("Invalid number of elements _(´ω`_)⌒)_ ))");

	loc.addRedirects(lines.at(1), lines.at(2));

	Result<std::string, bool> res = loc.getRedirects(lines.at(1));

	return (0);
}
