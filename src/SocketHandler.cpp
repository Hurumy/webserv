/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:26:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/26 11:07:22 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>

#include <algorithm>
#include <ctime>
#include <map>
#include <vector>

#include "CGIResponseCreator.hpp"
#include "CSocket.hpp"
#include "ConfParser.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "RequestHandler.hpp"
#include "Result.hpp"
#include "SSocket.hpp"
#include "puterror.hpp"

SocketHandler::SocketHandler() {}

SocketHandler::SocketHandler(std::vector<SSocket> &_ssockets,
							 std::size_t const _timeout, int _pollTimeout)
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

std::vector<CSocket>::iterator SocketHandler::_deinitCSocket(
	std::vector<CSocket>::iterator &csockiter) {
	int const csockfd(csockiter->getSockfd());

	std::map<int, CGIResponseCreator>::iterator cgiiter =
		cgiResponseCreators.find(csockfd);
	if (cgiiter != cgiResponseCreators.end()) {
		cgiiter->second.waitDeadCGIProc();
		cgiiter->second.deinit();
		cgiResponseCreators.erase(cgiiter);
	}
	std::map<int, Request>::iterator reqiter = requests.find(csockfd);
	if (reqiter != requests.end()) {
		requests.erase(reqiter);
	}
	std::map<int, Response>::iterator resiter = responses.find(csockfd);
	if (resiter != responses.end()) {
		responses.erase(resiter);
	}
	csockiter->closeSockfd();
	return csockets.erase(csockiter);
}

bool SocketHandler::removeClosedCSockets() {
	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end();) {
		// if ((iter->getRevents() & POLLHUP) == POLLHUP ||
		// 	(iter->getPhase() == CSocket::CLOSE) ||
		// 	(iter->getRevents() & POLLRDHUP) == POLLRDHUP) {
		if ((iter->getRevents() & POLLHUP) == POLLHUP ||
			(iter->getPhase() == CSocket::CLOSE)) {
			iter = _deinitCSocket(iter);
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

void SocketHandler::setSSockets(const std::vector<SSocket> &_ssockets) {
	ssockets = _ssockets;
}

std::vector<CSocket> const &SocketHandler::getCSockets() const {
	return csockets;
}

int SocketHandler::getTimeout() const { return pollTimeout; }

void SocketHandler::setTimeout(const std::size_t _timeout) {
	timeout = _timeout;
}

void SocketHandler::setPollTimeout(const int _pollTimeout) {
	pollTimeout = _pollTimeout;
}

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
			// added_pollfd.events = POLLIN | POLLOUT | POLLHUP | POLLRDHUP;
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
			pollfds.push_back(added_pollfd);
		}
	}
	if (requests.empty() == false) {
		for (std::map<int, CGIResponseCreator>::iterator iter =
				 cgiResponseCreators.begin();
			 iter != cgiResponseCreators.end(); ++iter) {
			if (iter->second.getPhase() == CGIResponseCreator::CGIWRITE ||
				iter->second.getPhase() == CGIResponseCreator::CGIRECV ||
				iter->second.getPhase() == CGIResponseCreator::CGILASTRECV) {
				std::memset(&added_pollfd, 0, sizeof(added_pollfd));
				added_pollfd.fd = iter->second.getMonitoredfd();
				added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
				pollfds.push_back(added_pollfd);
			}
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
				break;
			}
		}
		for (std::vector<CSocket>::iterator csockiter = csockets.begin();
			 csockiter != csockets.end(); ++csockiter) {
			if (polliter->fd == csockiter->getSockfd()) {
				csockiter->setRevents(polliter->revents);
				break;
			}
		}
		for (std::map<int, CGIResponseCreator>::iterator cgiiter =
				 cgiResponseCreators.begin();
			 cgiiter != cgiResponseCreators.end(); ++cgiiter) {
			CGIResponseCreator::tag _phase = cgiiter->second.getPhase();
			if ((_phase == CGIResponseCreator::CGIWRITE ||
				 _phase == CGIResponseCreator::CGIRECV ||
				 _phase == CGIResponseCreator::CGILASTRECV) &&
				polliter->fd == cgiiter->second.getMonitoredfd()) {
				cgiiter->second.setRevents(polliter->revents);
				break;
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
				putSytemError("accept");
				// return false;
			} else {
				fcntl(sockfd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
				csockets.push_back(CSocket(sockfd, s_addr.sin_addr.s_addr,
										   ssockiter->getIpaddr(),
										   ssockiter->getPort()));
			}
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

bool SocketHandler::sendDataMap(std::map<int, std::string> const &dataMap) {
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
	for (std::vector<CSocket>::iterator csockiter = csockets.begin();
		 csockiter != csockets.end(); ++csockiter) {
		if (csockiter->getPhase() == CSocket::SEND &&
			(csockiter->getRevents() & POLLOUT) == POLLOUT) {
			// for develop
			// responses[csockiter->getSockfd()].addHeader("Connection",
			// 											"Keep-Alive");
			std::stringstream ss;
			ss << responses[csockiter->getSockfd()].getBody().size();
			// for develope
			responses[csockiter->getSockfd()].addHeader("Content-Length",
														ss.str());
			if (csockiter->sendData(
					responses[csockiter->getSockfd()].getLines()) == false) {
				// error handling
			}
			// for develop
#if defined(_DEBUGFLAG)
			std::clog << responses[csockiter->getSockfd()].getLines()
					  << std::endl;
			{
				int fd = open("./Makefile", O_RDONLY);
				std::clog << "open fd: " << fd << std::endl;
				close(fd);
				std::clog << "the number of CGIResponseCreators: "
						  << cgiResponseCreators.size() << std::endl;
				std::clog << "the number of Response: " << responses.size()
						  << std::endl;
				std::clog << "the number of Request: " << requests.size()
						  << std::endl;
				std::clog << "the number of CSocket: " << csockets.size()
						  << std::endl;
			}
#endif
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
			if (requests.find(csockiter->getSockfd()) == requests.end()) {
				requests[csockiter->getSockfd()] = request;
				requests[csockiter->getSockfd()].setRemoteAddr(
					csockiter->getRemoteAddr());
				requests[csockiter->getSockfd()].setLocalAddr(
					csockiter->getLocalAddr());
				requests[csockiter->getSockfd()].setLocalPort(
					csockiter->getLocalPort());
			}
			if (requests[csockiter->getSockfd()].loadPayload(*csockiter) ==
				false) {
				// error handling
				// if request payload's format is invalid
			} else {
				// For developing CGI
#if defined(_DEBUGFLAG)
				std::clog << requests[csockiter->getSockfd()].getLines()
						  << std::endl;
#endif
				csockiter->setPhase(CSocket::PASS);
				// csockiter->setPhase(CSocket::CGI);
				// Response res;
				// res.setVersion("HTTP/1.1");
				// res.setStatus(200);
				// res.setStatusMessage("OK");
				// res.addHeader("Content-Type", "text/plain");
				// responses[csockiter->getSockfd()] = res;
				// CGIResponseCreator
				// cgiResponseCreator(requests[csockiter->getSockfd()],
				// responses[csockiter->getSockfd()], "/test/test.cgi");
				// cgiResponseCreator.setHostName("webserv");
				// cgiResponseCreator.setPortNum(8000);
				// cgiResponseCreators.insert(std::make_pair(csockiter->getSockfd(),
				// cgiResponseCreator));
			}
			csockiter->setLasttime(std::time(NULL));
		}
	}
	return true;
}

std::map<int, Request> const &SocketHandler::getRequests() const {
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
	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end(); ++iter) {
		if (iter->getPhase() == CSocket::PASS) {
			RequestHandler requestHandler =
				RequestHandler(configs, requests[iter->getSockfd()]);
// for debugging
#if defined(_DEBUGFLAG)
			std::clog << requests[iter->getSockfd()].getLines() << std::endl;
#endif
			if (requestHandler.searchMatchHost().isError() == true) {
				// error handling
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			} else if (requestHandler.checkRequiedHeader().isError() == true) {
				// error handling
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			} else if (requestHandler.routeMethod().isError() == true) {
				// error handling
				responses[iter->getSockfd()] = requestHandler.getResponse();
				iter->setPhase(CSocket::SEND);
				removeRequest(iter->getSockfd());
			} else {
				responses[iter->getSockfd()] = requestHandler.getResponse();
				if (requestHandler.isCgi().isOK() == true) {
					iter->setPhase(CSocket::CGI);
					CGIResponseCreator cgiResponseCreator(
						requests[iter->getSockfd()],
						responses[iter->getSockfd()],
						requestHandler.isCgi().getOk());
					cgiResponseCreator.setHostName(
						requestHandler.getHostname());
					cgiResponseCreator.setPortNum(
						requestHandler.getPortNumber());
					cgiResponseCreators.insert(
						std::make_pair(iter->getSockfd(), cgiResponseCreator));
				} else {
					iter->setPhase(CSocket::SEND);
					removeRequest(iter->getSockfd());
				}
			}
		}
	}
	return true;
}

bool SocketHandler::handleCGIRequest() {
	if (cgiResponseCreators.empty() == true) {
		return false;
	}
	for (std::map<int, CGIResponseCreator>::iterator iter =
			 cgiResponseCreators.begin();
		 iter != cgiResponseCreators.end();) {
		// Request &req = requests[iter->getSockfd()];
		switch (iter->second.getPhase()) {
			case CGIResponseCreator::CGISTARTUP: {
				// pipe(), fork(), execve()
				if (iter->second.execCGIScript() == false) {
					// error handling
				} else {
					// iter->second.setPhase(Request::CGIWRITE)
					iter->second.setPhase(CGIResponseCreator::CGIWRITE);
					// Set inpfd[1]to monitoredfd
					iter->second.setMonitoredfd(CGIResponseCreator::CGIWRITE);
				}
				++iter;
			} break;
			case CGIResponseCreator::CGIWRITE: {
				if ((iter->second.getRevents() & POLLOUT) != POLLOUT) {
					++iter;
					break;
				}
				if (iter->second.writeMessageBody() == true) {
					// iter->second.setPhase(Request::CGIRECV)
					iter->second.setPhase(CGIResponseCreator::CGIRECV);
					// Set outpfd[0]to monitoredfd
					iter->second.setMonitoredfd(CGIResponseCreator::CGIRECV);
				}
				++iter;
			} break;
			case CGIResponseCreator::CGIRECV: {
				if ((iter->second.getRevents() & POLLIN) == POLLIN) {
					// Reade output from outpfd[0]
					iter->second.recvCGIOutput();
					// deinit inpfd, outpfd, monitoredfd
					iter->second.setCGIOutput();
				}
				iter->second.waitChildProc();
				++iter;
			} break;
			case CGIResponseCreator::CGILASTRECV: {
				if ((iter->second.getRevents() & POLLIN) == POLLIN) {
					// Reade output from outpfd[0]
					iter->second.recvCGIOutput();
					// deinit inpfd, outpfd, monitoredfd
					iter->second.setCGIOutput();
				} else {
					iter->second.setPhase(CGIResponseCreator::CGIFIN);
				}
				++iter;
			} break;
			case CGIResponseCreator::CGIFIN: {
				for (std::vector<CSocket>::iterator csockiter =
						 csockets.begin();
					 csockiter != csockets.end(); ++csockiter) {
					if (csockiter->getSockfd() == iter->first) {
						csockiter->setPhase(CSocket::SEND);
						break;
					}
				}
				iter->second.deinit();
				std::map<int, CGIResponseCreator>::iterator erasedIter = iter;
				++iter;
				cgiResponseCreators.erase(erasedIter);
			} break;
		}
	}
	return true;
}

bool SocketHandler::closeTimeoutCSockets() {
	if (csockets.empty() == true) {
		return true;
	}
	for (std::vector<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end(); ++iter) {
		if (std::difftime(std::time(NULL), iter->getLasttime()) > timeout) {
			if (iter->getPhase() == CSocket::CGI) {
				std::map<int, CGIResponseCreator>::iterator cgiiter =
					cgiResponseCreators.begin();
				if (cgiiter != cgiResponseCreators.end()) {
					if (kill(cgiiter->second.getPid(), 0) == 0) {
						if (kill(cgiiter->second.getPid(), SIGTERM) == -1) {
							putSytemError("kill");
						}
					}
				}
			}
			iter->setPhase(CSocket::CLOSE);
		}
	}
	return true;
}
