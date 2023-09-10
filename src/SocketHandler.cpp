/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:26:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/10 19:11:44 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <poll.h>
#include <vector>
#include <map>
#include <netinet/in.h>

#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "SSocket.hpp"
#include "CSocket.hpp"

SocketHandler::SocketHandler(std::vector<SSocket> &_ssockets, int _timeout) : timeout(_timeout), ssockets(_ssockets) {}

bool SocketHandler::initAllSSockets() {
	for (std::vector<SSocket>::iterator iter = ssockets.begin(); iter != ssockets.end(); ++iter) {
		if (iter->init() == false) {
			// error handling
		}
	}
	return true;
}

bool SocketHandler::closeAllSSockets() {
	for (std::vector<SSocket>::iterator iter = ssockets.begin(); iter != ssockets.end(); ++iter) {
		if (iter->closeSockfd() == false) {
			// error handling
		}
	}
	return true;
}

bool SocketHandler::removeClosedCSockets() {
	for (std::vector<CSocket>::iterator iter = csockets.begin(); iter != csockets.end(); ++iter) {
		if ((iter->getRevents() & POLLHUP) == 1) {
			csockets.erase(iter);
		}
	}
	return true;
}

std::vector<SSocket> const &SocketHandler::getSSockets() const {
	return ssockets;
}

std::vector<CSocket> const &SocketHandler::getCSockets() const {
	return csockets;
}

int SocketHandler::getTimeout() const {
	return timeout;
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

void SocketHandler::clearPollfds() {
	pollfds.clear();
}

std::vector<struct pollfd> const &SocketHandler::getPollfds() const {
	return pollfds;
}

bool SocketHandler::setRevents() {
	if (poll(pollfds.data(), pollfds.size(), timeout) == -1) {
		return false;
	}
	for (std::vector<struct pollfd>::iterator polliter = pollfds.begin(); polliter != pollfds.end(); ++polliter) {
		for (std::vector<SSocket>::iterator ssockiter = ssockets.begin(); ssockiter != ssockets.end(); ++ssockiter) {
			if (polliter->fd == ssockiter->getSockfd()) {
				ssockiter->setRevents(polliter->revents);
			}
		}
	}
	for (std::vector<struct pollfd>::iterator polliter = pollfds.begin(); polliter != pollfds.end(); ++polliter) {
		for (std::vector<CSocket>::iterator csockiter = csockets.begin(); csockiter != csockets.end(); ++csockiter) {
			if (polliter->fd == csockiter->getSockfd()) {
				csockiter->setRevents(polliter->revents);
			}
		}
	}
	return true;
}

bool SocketHandler::recieveCSockets() {
	struct  sockaddr_in s_addr;
	socklen_t addrsize;
	int sockfd;

	for (std::vector<SSocket>::iterator ssockiter = ssockets.begin(); ssockiter != ssockets.end(); ++ssockiter) {
		if ((ssockiter->getRevents() & POLLIN) == 1) {
			std::memset(&s_addr, 0, sizeof(s_addr));
			addrsize = sizeof(sockaddr_in);
			sockfd = accept(ssockiter->getSockfd(), (struct sockaddr *)&s_addr, (socklen_t *)&addrsize);
			if (sockfd == -1) {
				// error handling?
				return false;
			}
			csockets.push_back(CSocket(sockfd));
		}
	}
	return true;
}

Result<std::map<int, std::string>, bool> SocketHandler::getDataMap() const {
	std::map<int, std::string> dataMap;

	if (csockets.empty() == true) {
		return Error<bool>(false);
	}
	for (std::vector<CSocket>::const_iterator iter = csockets.begin(); iter != csockets.end(); ++iter) {
		if ((iter->getRevents() & POLLIN) == 1) {
			Result<std::string, bool> data = iter->getData();
			if (data.isOK() == true) {
				dataMap[iter->getSockfd()] = data.getOk();
			} else {
				// error handling
				// return Error<bool>(false);
			}
		}
	}
	if (dataMap.empty() == true) {
		return Error<bool>(false);
	}
	return Ok<std::map<int, std::string>>(dataMap);
}
