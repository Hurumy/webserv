/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:26:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/05 21:09:26 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>

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

std::list<CSocket>::iterator SocketHandler::_deinitCSocket(
	std::list<CSocket>::iterator &csockiter) {
	int const csockfd(csockiter->getSockfd());
	pid_t cpid;

	std::map<int, CGIResponseCreator>::iterator cgiiter =
		cgiResponseCreators.find(csockfd);
	if (cgiiter != cgiResponseCreators.end()) {
		cpid = cgiiter->second.getPid();
		if (cpid != 0) { cpids.push_back(cgiiter->second.getPid()); }
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
	short pollhup;

#if defined(_LINUX)
	pollhup = POLLRDHUP;
#elif defined(_DARWIN)
	pollhup = POLLHUP;
#else
	pollhup = POLLHUP;
#endif
	for (std::list<CSocket>::iterator iter = csockets.begin();
		 iter != csockets.end();) {
		if ((iter->getRevents() & pollhup) == pollhup ||
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

std::list<CSocket> const &SocketHandler::getCSockets() const {
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
		for (std::list<CSocket>::iterator iter = csockets.begin();
			 iter != csockets.end(); ++iter) {
			std::memset(&added_pollfd, 0, sizeof(added_pollfd));
			added_pollfd.fd = iter->getSockfd();
#if defined(_LINUX)
			added_pollfd.events = POLLIN | POLLOUT | POLLRDHUP;
#elif defined(_DARWIN)
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
#else
			added_pollfd.events = POLLIN | POLLOUT | POLLHUP;
#endif
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
		for (std::list<CSocket>::iterator csockiter = csockets.begin();
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
	for (std::list<CSocket>::iterator iter = csockets.begin();
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

bool SocketHandler::sendResponses() {
	if (responses.empty() == true) {
		return false;
	}
	for (std::list<CSocket>::iterator csockiter = csockets.begin();
		 csockiter != csockets.end(); ++csockiter) {
		if ((csockiter->getPhase() == CSocket::SEND || csockiter->getPhase() == CSocket::CSENDERROR) &&
			(csockiter->getRevents() & POLLOUT) == POLLOUT) {
			if (csockiter->sendData(
					responses[csockiter->getSockfd()].getLines()) == false) {
				// error handling
			} else {
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
				if (csockiter->getPhase() == CSocket::CSENDERROR) {
					csockiter->setPhase(CSocket::CLOSE);
				} else {
					csockiter->setPhase(CSocket::RECV);
				}
				removeResponse(csockiter->getSockfd());
			}
		}
	}
	return true;
}

bool SocketHandler::loadRequests() {
	Request request;

	if (csockets.empty() == true) {
		return false;
	}
	for (std::list<CSocket>::iterator csockiter = csockets.begin();
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
				if (csockiter->getPhase() == CSocket::CLOSE) {
					
				}
			} else {
				// For developing CGI
#if defined(_DEBUGFLAG)
				std::clog << requests[csockiter->getSockfd()].getLines()
						  << std::endl;
#endif
				csockiter->setPhase(CSocket::PASS);
			}
			csockiter->setLasttime(std::time(NULL));
		}
	}
	return true;
}

std::map<int, Request> const &SocketHandler::getRequests() const {
	return requests;
}

bool SocketHandler::loadResponses(std::vector<Config> const &configs) {
	for (std::list<CSocket>::iterator iter = csockets.begin();
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
					if (cgiResponseCreators.find(iter->getSockfd()) == cgiResponseCreators.end()) {
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
					}
				} else {
					iter->setPhase(CSocket::SEND);
					removeRequest(iter->getSockfd());
				}
			}
		} else if (iter->getPhase() == CSocket::CSETERROR) {
			Response response;
			response.setVersion("HTTP/1.1");
			response.setStatus(400);
			response.setStatusMessage("Bad Request");
			response.setHeader("Connecion", "close");
			responses[iter->getSockfd()] = response;
			iter->setPhase(CSocket::CSENDERROR);
		}
	}
	return true;
}

bool SocketHandler::handleCGIRequest(std::vector<Config> const &configs) {
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
					iter->second.setPhase(CGIResponseCreator::CGIFIN);
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
					iter->second.waitChildProc();
					++iter;
					break;
				}
				if (iter->second.writeMessageBody() == true) {
					// iter->second.setPhase(Request::CGIRECV)
					iter->second.setPhase(CGIResponseCreator::CGIRECV);
					// Set outpfd[0]to monitoredfd
					iter->second.setMonitoredfd(CGIResponseCreator::CGIRECV);
				}
				iter->second.waitChildProc();
				++iter;
			} break;
			case CGIResponseCreator::CGIRECV: {
				if ((iter->second.getRevents() & POLLIN) == POLLIN) {
					// Reade output from outpfd[0]
					iter->second.recvCGIOutput();
					// deinit inpfd, outpfd, monitoredfd
					// iter->second.setCGIOutput(configs);
				}
				iter->second.waitChildProc();
				++iter;
			} break;
			case CGIResponseCreator::CGILASTRECV: {
				if ((iter->second.getRevents() & POLLIN) == POLLIN) {
					// Reade output from outpfd[0]
					iter->second.recvCGIOutput();
					// deinit inpfd, outpfd, monitoredfd
					// iter->second.setCGIOutput(configs);
				} else {
					iter->second.setPhase(CGIResponseCreator::CGIFIN);
				}
				++iter;
			} break;
			case CGIResponseCreator::CGIFIN: {
				iter->second.setCGIOutput(configs);
				for (std::list<CSocket>::iterator csockiter =
						 csockets.begin();
					 csockiter != csockets.end(); ++csockiter) {
					if (csockiter->getSockfd() == iter->first) {
						if (iter->second.getResponseType() == CGIResponseCreator::LOCALREDIR) {
							csockiter->setPhase(CSocket::PASS);
						} else {
							csockiter->setPhase(CSocket::SEND);
						}
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
	for (std::list<CSocket>::iterator iter = csockets.begin();
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

bool SocketHandler::waitDeadCGIProcs() {
	int wstatus;

	if (cpids.empty() == true) { return true; }
	for (std::list<pid_t>::iterator iter = cpids.begin(); iter != cpids.end();) {
		if (*iter == 0 || kill(*iter, 0) == -1) { iter = cpids.erase(iter);}
		else {
			switch (waitpid(*iter, &wstatus, WNOHANG)) {
				case -1: { putSytemError("waitpid"); ++iter; } break;
				case 0: { ++iter; } break;
				default : { iter = cpids.erase(iter); } break;
			}
		}
	}
	return true;
}
