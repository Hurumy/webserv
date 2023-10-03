/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:26:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/03 21:23:59 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>

#include <algorithm>
#include <map>
#include <vector>
#include <ctime>

#include "CSocket.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "SSocket.hpp"
#include "ConfParser.hpp"
#include "RequestHandler.hpp"
#include "puterror.hpp"

SocketHandler::SocketHandler(std::vector<SSocket> &_ssockets, std::size_t const _timeout, int _pollTimeout)
	: ssockets(_ssockets), timeout(_timeout), pollTimeout(_pollTimeout) {}

bool SocketHandler::initAllSSockets() {
	for (std::vector<SSocket>::iterator iter = ssockets.begin();
		 iter != ssockets.end(); ++iter) {
		if (iter->init() == false) {
			// error handling
		}
	}
	return true;
}

bool SocketHandler::closeAllSSockets() {
	for (std::vector<SSocket>::iterator iter = ssockets.begin();
		 iter != ssockets.end(); ++iter) {
		if (iter->closeSockfd() == false) {
			// error handling
		}
	}
	return true;
}

bool SocketHandler::removeClosedCSockets() {
	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end();) {
		if ((iter->getRevents() & POLLHUP) ==
				POLLHUP ||
			(iter->getPhase() == CSocket::CLOSE)) {
			iter->closeSockfd();
			iter = csockets.erase(iter);
		} else {
			iter++;
		}
	}
	return true;
}

bool SocketHandler::removeRequest(int const csockfd) {
	std::map<int, Request>::iterator iter = requests.find(csockfd);
	if (iter == requests.end()) {
		return false;
	}
	requests.erase(iter);
	return true;
}

bool SocketHandler::removeResponse(int const csockfd) {
	std::map<int, Response>::iterator iter = responses.find(csockfd);
	if (iter == responses.end()) {
		return false;
	}
	responses.erase(iter);
	return true;
}

std::vector<SSocket> const &SocketHandler::getSSockets() const {
	return ssockets;
}

std::vector<CSocket> const &SocketHandler::getCSockets() const {
	return csockets;
}

int SocketHandler::getTimeout() const { return pollTimeout; }

void SocketHandler::addCSocket(CSocket const &_csocket) {
	csockets.push_back(_csocket);
}

bool SocketHandler::createPollfds() {
	struct pollfd added_pollfd;
	if (ssockets.empty() == false) {
		for (std::vector<SSocket>::iterator iter = ssockets.begin();
			 iter != ssockets.end(); ++iter) {
			std::memset(&added_pollfd, 0, sizeof(added_pollfd));
			added_pollfd.fd = iter->getSockfd();
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
			pollfds.push_back(added_pollfd);
		}
	}
	if (csockets.empty() == false) {
		for (std::vector<CSocket>::iterator iter = csockets.begin();
			 iter != csockets.end(); ++iter) {
			std::memset(&added_pollfd, 0, sizeof(added_pollfd));
			added_pollfd.fd = iter->getSockfd();
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
			pollfds.push_back(added_pollfd);
		}
	}
	return true;
}

void SocketHandler::clearPollfds() { pollfds.clear(); }

std::vector<struct pollfd> const &SocketHandler::getPollfds() const {
	return pollfds;
}

bool SocketHandler::setRevents() {
	if (poll(pollfds.data(), pollfds.size(), pollTimeout) == -1) {
		putSytemError("poll");
		std::exit(EXIT_FAILURE);
	}
	for (std::vector<struct pollfd>::iterator polliter = pollfds.begin();
		 polliter != pollfds.end(); ++polliter) {
		for (std::vector<SSocket>::iterator ssockiter = ssockets.begin();
			 ssockiter != ssockets.end(); ++ssockiter) {
			if (polliter->fd == ssockiter->getSockfd()) {
				ssockiter->setRevents(polliter->revents);
			}
		}
	}
	for (std::vector<struct pollfd>::iterator polliter = pollfds.begin();
		 polliter != pollfds.end(); ++polliter) {
		for (std::vector<CSocket>::iterator csockiter = csockets.begin();
			 csockiter != csockets.end(); ++csockiter) {
			if (polliter->fd == csockiter->getSockfd()) {
				csockiter->setRevents(polliter->revents);
			}
		}
	}
	return true;
}

bool SocketHandler::recieveCSockets() {
	struct sockaddr_in s_addr;
	socklen_t addrsize;
	int sockfd;

	for (std::vector<SSocket>::iterator ssockiter = ssockets.begin();
		 ssockiter != ssockets.end(); ++ssockiter) {
		if ((ssockiter->getRevents() & POLLIN) == POLLIN) {
			std::memset(&s_addr, 0, sizeof(s_addr));
			addrsize = sizeof(sockaddr_in);
			sockfd = accept(ssockiter->getSockfd(), (struct sockaddr *)&s_addr,
							(socklen_t *)&addrsize);
			if (sockfd == -1) {
				// error handling?
				return false;
			}
			fcntl(sockfd, O_NONBLOCK);
			csockets.push_back(CSocket(sockfd));
		}
	}
	return true;
}

bool SocketHandler::recvCSocketsData() {
	if (csockets.empty() == true) {
		return false;
	}
	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end(); ++iter) {
		if ((iter->getRevents() & POLLIN) == POLLIN &&
			iter->getPhase() == CSocket::RECV) {
			if (iter->readData() == false) {
				return false;
			}
			std::string data = iter->getData();
			std::map<int, Request>::iterator reqiter =
				requests.find(iter->getSockfd());
			if (reqiter != requests.end() &&
				reqiter->second.getPhase() == Request::BODY) {
				iter->setPhase(CSocket::LOAD);
			} else if (data.find("\r\n") != std::string::npos) {
				iter->setPhase(CSocket::LOAD);
			}
		}
	}
	return true;
}

bool SocketHandler::sendDataMap(
	std::map<int, std::string> const &dataMap) {
	if (dataMap.empty() == true) {
		// error handling
		return false;
	}
	for (std::map<int, std::string>::const_iterator dataiter = dataMap.begin();
		 dataiter != dataMap.end(); ++dataiter) {
		for (std::vector<CSocket>::iterator csockiter = csockets.begin();
			 csockiter != csockets.end(); ++csockiter) {
			if ((csockiter->getRevents() & POLLOUT) == POLLOUT) {
				if (dataiter->first == csockiter->getSockfd()) {
					if (csockiter->sendData(dataiter->second) == false) {
						// error handling
					}
					csockiter->setPhase(CSocket::RECV);
					break;
				}
			}
		}
	}
	return true;
}

bool SocketHandler::sendResponses() {
	if (responses.empty() == true) {
		return false;
	}
	for (std::vector<CSocket>::iterator csockiter = csockets.begin(); csockiter != csockets.end(); ++csockiter) {
		if (csockiter->getPhase() == CSocket::SEND && (csockiter->getRevents() & POLLOUT) == POLLOUT) {
			if (csockiter->sendData(responses[csockiter->getSockfd()].getLines()) == false) {
				// error handling
			}
			csockiter->setPhase(CSocket::RECV);
			removeResponse(csockiter->getSockfd());
		}
	}
	return true;
}

bool SocketHandler::loadRequests() {
	Request request;

	if (csockets.empty() == true) {
		return false;
	}
	for (std::vector<CSocket>::iterator csockiter = csockets.begin();
		 csockiter != csockets.end(); ++csockiter) {
		if ((csockiter->getRevents() & POLLIN) == POLLIN &&
			csockiter->getPhase() == CSocket::LOAD) {
			std::map<int, Request>::iterator reqiter =
				requests.find(csockiter->getSockfd());
			if (reqiter == requests.end()) {
				requests[csockiter->getSockfd()] = request;
			}
			if (requests[csockiter->getSockfd()].loadPayload(*csockiter) ==
				false) {
				// error handling
				// if request payload's format is invalid
			}
			csockiter->setLasttime(std::time(NULL));
		}
	}
	return true;
}

std::map<int, Request> SocketHandler::getRequestsMap() const {
	return requests;
}

std::map<int, std::string> SocketHandler::createResponse() {
	std::map<int, std::string> response;

	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end(); ++iter) {
		if (iter->getPhase() == CSocket::PASS) {
			response[iter->getSockfd()] = std::string(
				"HTTP/1.1 200 OK\r\nContent-Type: "
				"text/plain\r\nContent-Length: 14\r\n\r\nHello world!\r\n");
			removeRequest(iter->getSockfd());
		}
	}
	return response;
}

bool SocketHandler::loadResponses(std::vector<Config> const &configs) {
	// temporarily
	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end(); ++iter) {
		if (iter->getPhase() == CSocket::PASS) {
			RequestHandler requestHandler = RequestHandler(configs, requests[iter->getSockfd()]);
			if (requestHandler.searchMatchHost().isError() == true) {
				// error handling	
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			}
			else if (requestHandler.checkRequiedHeader().isError() == true) {
				// error handling	
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			}
			else if (requestHandler.routeMethod().isError() == true) {
				// error handling	
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			} else {
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			}
		}
	}
	return true;
}

bool SocketHandler::closeTimeoutCSockets() {
	if (csockets.empty() == true) {
		return true;
	}
	for (std::vector<CSocket>::iterator iter = csockets.begin(); iter != csockets.end(); ++iter) {
		if (std::difftime(std::time(NULL), iter->getLasttime()) > timeout) {
			iter->setPhase(CSocket::CLOSE);
		}
	}
	return true;
}
