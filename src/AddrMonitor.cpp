/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrMonitor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:50:29 by shtanemu          #+#    #+#             */
/*   Updated: 2023/12/25 11:08:39 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AddrMonitor.hpp"

AddrMonitor::AddrMonitor() 
    : lasttime(std::time(NULL)) {}

bool AddrMonitor::countAddr(std::string &remoteAddr) {
	std::map<std::string, std::size_t>::iterator iter;

	iter = addrMap.find(remoteAddr);
	if (iter != addrMap.end()) {
		iter->second++;
	} else {
		addrMap[remoteAddr] = 0;
	}
    return true;    
}

bool AddrMonitor::clearAddrMap() {
    if (std::difftime(std::time(NULL), lasttime) > 5) {
	    addrMap.clear();
    }
    return true;
}

bool AddrMonitor::IsWarn() {
    for (std::map<std::string, std::size_t>::iterator iter = addrMap.begin(); iter != addrMap.end(); ++iter) {
        if (iter->second > 3000) { return true; }
    }
    return false;
}