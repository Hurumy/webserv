/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:15:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/26 11:08:39 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#include <string>

typedef uint32_t u_int32_t;

enum ipvers {
		IPV4,
		IPV6,
};

class SSocket {
	private:
		SSocket();
		u_int32_t _convertIpstrToUint() const;

		int sockfd;
		std::string ipaddr;
		unsigned int port;
		ipvers ipver;
		int backlog;
		short revents;

	protected:
	public:
		SSocket(const std::string &_ipaddr, unsigned int _port, ipvers _ipver, int _backlog);

		bool init();
		bool closeSockfd();
		unsigned int getPort() const;
		int getSockfd() const;
		std::string const &getIpaddr() const;
		ipvers getIpVer() const;
		int getBacklog() const;
		short getRevents() const;
		void setRevents(short const _revents);
};
