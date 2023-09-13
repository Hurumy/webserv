/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_return.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:59:23 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/13 16:22:35 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

// static int	threecontents(Config &conf, std::vector<std::string> lines)
// {
	
// }

// static int	twocontents(Config &conf, std::vector<std::string> lines)
// {
// 	std::stringstream	ss;
// 	int					num;

// 	if (isNumber(lines.at(1)) == true)
// 	{
// 		ss << lines.at(1);
// 		ss >> num;
// 	}
// 	else
// 	{
		
// 	}
// }

// int readRedirect(Config &conf, std::string oneline)
// {
// 	std::vector<std::string>	lines;
// 	std::stringstream			ss;
// 	int							num;

// 	lines = lineSpliter(oneline, " ");

// 	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

// 	if (lines.at(0) != "return")
// 		errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

// 	if (lines.size() == 2)
// 		return (twocontents(conf, lines));
// 	else if (lines.size() == 3)
// 		return (threecontents(conf, lines));
// 	else
// 		errorInInit("Invalid form detected in return directives ⊂('ω`⊂ 三");

// 	return (0);
// }

