/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_root.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:55:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/13 13:04:46 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readRoot(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "root")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit("Too many Root directives _(´ω`_)⌒)_ ))");

	conf.setRootDir(lines.at(1));
	std::cout << CYAN "root: " << conf.getRootDir() << RESET << std::endl;

	return (0);
}
