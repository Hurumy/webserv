/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   puterror.cpp                                          :+:      :+:    :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 08:45:11 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/27 19:26:18 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>

#include "webserv.hpp"

int errorInInit(std::string errormessage)
{
	std::string tmp;
	tmp = RED + errormessage + RESET;
	perror(tmp.c_str());
	std::exit(1);
}

void putSytemError(char const *msg) {
	std::cerr << RED << "webserv: error: " << msg << ": " << strerror(errno)
			  << RESET << std::endl;
}
