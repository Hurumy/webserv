/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_rewrite.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:23:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/14 16:43:45 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readRewrite(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "rewrite")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 3)
		errorInInit("Too many Root directives _(´ω`_)⌒)_ ))");

	conf.addRedirects(lines.at(1), lines.at(2));

	Result<std::string, bool>	res = conf.getRedirects(lines.at(1));
	//std::cout << YELLOW "redirects: " << lines.at(1) << ": " << res.getOk() << RESET << std::endl;

	return (0);
}


