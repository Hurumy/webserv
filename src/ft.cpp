/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:03:08 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/07 12:46:02 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.hpp"

#include <errno.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <string>

#include "webserv.hpp"

namespace ft {

static bool predUCharCaseIns(const char &c1, const char &c2) {
	return std::tolower(static_cast<unsigned char>(c1)) ==
		   std::tolower(static_cast<unsigned char>(c2));
}

bool strcmpCaseIns(const std::string &s1, const std::string &s2) {
	return s1.size() == s2.size() &&
		   std::equal(s1.begin(), s1.end(), s2.begin(), predUCharCaseIns);
}

int errorInInit(std::string const &errormessage) {
	std::string tmp;
	tmp = RED + errormessage + RESET;
	perror(tmp.c_str());
	std::exit(1);
}

void putSystemError(char const *msg) {
	std::cerr << RED << "webserv: error: " << msg << ": "
			  << std::strerror(errno) << RESET << std::endl;
}
}  // namespace ft
