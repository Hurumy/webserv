/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:26:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 14:17:48 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <vector>
#include "SSocket.hpp"
#include "CSocket.hpp"

SocketHandler::SocketHandler(std::vector<SSocket> &_ssockets) : ssockets(_ssockets) {}

std::vector<SSocket> const &SocketHandler::getSSockets() const {
	return ssockets;
}

std::vector<CSocket> const &SocketHandler::getCSockets() const {
	return csockets;
}

void SocketHandler::addCSocket(CSocket const &_csocket) {
	csockets.push_back(_csocket);
}

bool SocketHandler::createPollfds() {
	struct pollfd added_pollfd;
	if (ssockets.empty() == false) {
		for (std::vector<SSocket>::iterator iter = ssockets.begin(); iter != ssockets.end(); ++iter) {
			std::memset(&added_pollfd, 0, sizeof(added_pollfd));
			added_pollfd.fd = iter->getSockfd();
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
			pollfds.push_back(added_pollfd);
		}
	}
	if (csockets.empty() == false) {
		for (std::vector<CSocket>::iterator iter = csockets.begin(); iter != csockets.end(); ++iter) {
			std::memset(&added_pollfd, 0, sizeof(added_pollfd));
			added_pollfd.fd = iter->getSockfd();
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
			pollfds.push_back(added_pollfd);
		}	
	}
	return true;
}

std::vector<struct pollfd> const &SocketHandler::getPollfds() const {
	return pollfds;
}
