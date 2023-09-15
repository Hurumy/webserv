/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_allowedmethods.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:01:52 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 18:19:00 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readAllowedMethods(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "allowedMethods")
		errorInInit("Unknown directive was detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() > 5)
		errorInInit("Too many allowedMethods directives _(´ω`_)⌒)_ ))");

	for (size_t i = 1; i < lines.size(); i ++)
	{
		if (lines.at(i) == "GET" || lines.at(i) == "POST" || lines.at(i) == "DELETE" || lines.at(i) == "PUT")
			conf.addReqMethod(lines.at(i), true);
		else
			errorInInit("Unsupported method is detected (*´ω｀*)");
	}

	return (0);
}

