/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:48:37 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 12:03:04 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

SSocket::SSocket(int _port, ipvers _ipver, int _backlog) : port(_port), ipver(_ipver), backlog(_backlog) {}

bool SSocket::init() {
	struct  sockaddr_in s_addr;
	socklen_t addrsize;

	std::memset(&s_addr, 0, sizeof(sockaddr_in));
	if (ipver == IPV4) {
		s_addr.sin_family = AF_INET;
	} else if (ipver == IPV6) {
		s_addr.sin_family = AF_INET6;
	} else {
		// how to handle error for this?
		return false;
	}
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = INADDR_ANY;
	addrsize = sizeof(s_addr);
	sockfd = socket(s_addr.sin_family, SOCK_STREAM, 0);
	if (sockfd == -1) {
		// error output
		return false;
	}
	if (bind(sockfd, (const struct sockaddr *)&s_addr, addrsize) == -1) {
		// error output
		return false;
	}
	if (listen(sockfd, backlog) == -1) {
		// error output
		return false;
	}
	return true;
}

int const &SSocket::getSockfd() const {
	return sockfd;
}

ipvers const &SSocket::getIpVer() const {
	return ipver;
}

int const &SSocket::getPort() const {
	return port;
}

int const &SSocket::getBacklog() const {
	return backlog;
}
