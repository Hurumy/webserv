/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrMonitor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 10:50:33 by shtanemu          #+#    #+#             */
/*   Updated: 2023/12/25 20:19:44 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#include <ctime>
#include <map>
#include <string>

typedef uint32_t u_int32_t;

class AddrMonitor {
	private:
		std::map<std::string, std::size_t> addrMap;
		std::time_t lasttime;

		std::string _getRemoteAddr(u_int32_t s_addr);

	protected:
	public:
		AddrMonitor();

		bool countAddr(unsigned long s_addr);
		bool clearAddrMap();
		bool IsWarn();
};