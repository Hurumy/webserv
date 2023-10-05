/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_root.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:55:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/05 12:38:41 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readRoot(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	//std::cout << oneline << std::endl;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "root")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit("Too many Root directives _(´ω`_)⌒)_ ))");

	if (conf.getRootDir().empty() == false)
		errorInInit("Too many root declare");

	conf.setRootDir(lines.at(1));

	return (0);
}

int l_readRoot(Location &loc, std::string oneline)
{
	std::vector<std::string> lines;

	//std::cout << oneline << std::endl;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "root")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit("Too many Root directives _(´ω`_)⌒)_ ))");

	if (loc.getRootDir().empty() == false)
		errorInInit("Too many root declare");

	loc.setRootDir(lines.at(1));

	return (0);
}
