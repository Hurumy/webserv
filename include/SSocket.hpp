/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:15:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/26 10:03:08 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

enum ipvers {
		IPV4,
		IPV6,
};

class SSocket {
	private:
		SSocket();

		int sockfd;
		std::string ipaddr;
		int port;
		ipvers ipver;
		int backlog;
		short revents;

	protected:
	public:
		SSocket(const std::string &_ipaddr, int _port, ipvers _ipver, int _backlog);

		bool init();
		bool closeSockfd();
		int getPort() const;
		int getSockfd() const;
		ipvers getIpVer() const;
		int getBacklog() const;
		short getRevents() const;
		void setRevents(short const _revents);
};
