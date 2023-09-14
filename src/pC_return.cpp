/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_return.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:59:23 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/13 17:46:53 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static int	threecontents()
{
	std::cout << RED "RETURN: Implement later with updating conf class!!" RESET << std::endl;
	return (0);
}

static int	twocontents()
{
	// std::stringstream	ss;
	// int					num;

	// if (isNumber(lines.at(1)) == true)
	// {
	// 	ss << lines.at(1);
	// 	ss >> num;
	// }
	// else
	// {
		
	// }

	std::cout << RED "RETURN: Implement later with updating conf class!!" RESET << std::endl;
	return (0);
}

int readReturn(Config &conf, std::string oneline)
{
	std::vector<std::string>	lines;

	lines = lineSpliter(oneline, " ");

	conf.getPort();

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "return")
		errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() == 2)
		return (twocontents());
	else if (lines.size() == 3)
		return (threecontents());
	else
		errorInInit("Invalid form detected in return directives ⊂('ω`⊂ 三");

	return (0);
}

