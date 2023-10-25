/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_alias.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 11:34:51 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/23 11:44:37 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int l_readAlias(Location &loc, std::string oneline)
{
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "alias")
		errorInInit("Unknown directive was detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit(
			"Too many elements in alias directives _(´ω`_)⌒)_ ))");

	loc.setAlias(lines.at(1));

	return (0);
}


