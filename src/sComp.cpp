/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sComp.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:15:07 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/05 14:42:01 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sComp.hpp"

unsigned char sComp::helper_tolower(unsigned char c) {
	if ('A' <= c && c <= 'Z')
		return (c - 'A' + 'a');
	else
		return (c);
}

bool sComp::operator()(std::string const &s1, std::string const &s2) const {
	std::string str1(s1.length(), ' ');
	std::string str2(s2.length(), ' ');
	std::transform(s1.begin(), s1.end(), str1.begin(), sComp::helper_tolower);
	std::transform(s2.begin(), s2.end(), str2.begin(), sComp::helper_tolower);
	return (str1 < str2);
}
