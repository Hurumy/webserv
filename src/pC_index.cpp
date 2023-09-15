/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_index.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:20:57 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 17:31:50 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readIndex(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "index")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		errorInInit("Too many Index directives _(´ω`_)⌒)_ ))");

	if (conf.getReturnDir().empty() == false)
		errorInInit("Too many index declare");

	//後でRecursiveにする
	conf.setReturnDir(lines.at(1));

	return (0);
}

