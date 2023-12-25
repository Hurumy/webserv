/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrMonitor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:50:33 by shtanemu          #+#    #+#             */
/*   Updated: 2023/12/25 11:08:06 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <string>
#include <map>

class AddrMonitor {
    private:
        std::map<std::string, std::size_t> addrMap;
        std::time_t lasttime;
    protected:
    public:
        AddrMonitor();

		bool countAddr(std::string &remoteAddr);
		bool clearAddrMap();
		bool IsWarn();
};