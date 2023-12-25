/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrMonitor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:50:29 by shtanemu          #+#    #+#             */
/*   Updated: 2023/12/25 20:19:27 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AddrMonitor.hpp"

#include <stdint.h>

#include <sstream>

typedef uint32_t u_int32_t;

AddrMonitor::AddrMonitor() : lasttime(std::time(NULL)) {}

std::string AddrMonitor::_getRemoteAddr(u_int32_t s_addr) {
	int byte;
	int bitshift(0);
	std::stringstream ss;
	std::string remoteAddr;

	while (4 > bitshift) {
		byte = (s_addr >> (bitshift * 8)) & 0xFF;
		ss << byte;
		if (bitshift == 3) {
			remoteAddr.append(ss.str());
		} else {
			remoteAddr.append(ss.str() + ".");
		}
		ss.str("");
		ss.clear(std::stringstream::goodbit);
		bitshift++;
	}
	return remoteAddr;
}

bool AddrMonitor::countAddr(unsigned long s_addr) {
	std::map<std::string, std::size_t>::iterator iter;
	std::string remoteAddr(_getRemoteAddr(s_addr));

	iter = addrMap.find(remoteAddr);
	if (iter != addrMap.end()) {
		iter->second++;
	} else {
		addrMap[remoteAddr] = 0;
	}
	return true;
}

bool AddrMonitor::clearAddrMap() {
	if (std::difftime(std::time(NULL), lasttime) > 8) {
		addrMap.clear();
		lasttime = std::time(NULL);
	}
	return true;
}
#include "webserv.hpp"
bool AddrMonitor::IsWarn() {
	for (std::map<std::string, std::size_t>::iterator iter = addrMap.begin();
		 iter != addrMap.end(); ++iter) {
		if (iter->second > 3000) {
			return true;
		}
	}
	return false;
}