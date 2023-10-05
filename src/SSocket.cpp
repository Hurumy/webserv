/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:48:37 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/28 00:29:07 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SSocket.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

#include "puterror.hpp"

SSocket::SSocket(int _port, ipvers _ipver, int _backlog)
	: port(_port), ipver(_ipver), backlog(_backlog), revents(0) {}

bool SSocket::init() {
	struct sockaddr_in s_addr;
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
		putSytemError("socket");
		std::exit(EXIT_FAILURE);
	}
	if (bind(sockfd, (const struct sockaddr *)&s_addr, addrsize) == -1) {
		// error output
		putSytemError("bind");
		std::exit(EXIT_FAILURE);
	}
	if (listen(sockfd, backlog) == -1) {
		// error output
		putSytemError("listen");
		std::exit(EXIT_FAILURE);
	}
	return true;
}

bool SSocket::closeSockfd() {
	if (close(sockfd) == -1) {
		return false;
	}
	return true;
}

int SSocket::getSockfd() const { return sockfd; }

ipvers SSocket::getIpVer() const { return ipver; }

int SSocket::getPort() const { return port; }

int SSocket::getBacklog() const { return backlog; }

short SSocket::getRevents() const { return revents; }

void SSocket::setRevents(short const _revents) { revents = _revents; }
