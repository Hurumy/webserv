/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringCleaner.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:06:54 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/12 11:13:23 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool	isNumber(std::string str)
{
	for (size_t i = 0; i < str.length(); i ++)
	{
		if (!('0' <= str.c_str()[i] && str.c_str()[i] <= '9'))
			return (false);
	}
	return (true);
}

bool	isSpace(std::string str)
{
	char c;

	for (size_t i = 0; i < str.length(); i ++)
	{
		c = str.c_str()[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n' && c != '\v' && c != '\f')
			return (false);
	}
	return (true);
}

bool	isCapitalAlphabet(std::string str)
{
	for (size_t i = 0; i < str.length(); i ++)
	{
		if (!('A' <= str.c_str()[i] && str.c_str()[i] <= 'Z'))
			return (false);
	}
	return (true);
}

bool	isSmallAlphabet(std::string str)
{
	for (size_t i = 0; i < str.length(); i ++)
	{
		if (!('a' <= str.c_str()[i] && str.c_str()[i] <= 'z'))
			return (false);
	}
	return (true);
}

bool	isAlphabet(std::string str)
{
	for (size_t i = 0; i < str.length(); i ++)
	{
		if (!('a' <= str.c_str()[i] && str.c_str()[i] <= 'z') && !('A' <= str.c_str()[i] && str.c_str()[i] <= 'Z'))
			return (false);
	}
	return (true);
}

bool	isAlnum(std::string str)
{
	for (size_t i = 0; i < str.length(); i ++)
	{
		if (!('a' <= str.c_str()[i] && str.c_str()[i] <= 'z') && !('A' <= str.c_str()[i] && str.c_str()[i] <= 'Z')
				&& !('0' <= str.c_str()[i] && str.c_str()[i] <= '9'))
			return (false);
	}
	return (true);
}

// bool	removeEmptyString(std::vector<std::string> lines)
// {
// 	for (size_t i = 0; i < lines.size(); i ++)
// 	{
// 		if (lines.at(i).empty() == true)
// 			lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());
// 	}
// }

// int	main()
// {
// 	bool	test;
// 	std::string	str;

// 	test = isAlnum("dawdw2089eudqw!oejiqoDAJKSHENDE");
// 	if (test == true)
// 		str = "true";
// 	else
// 		str = "false";

// 	std::cout << str << std::endl;

// 	return 0;
// }
