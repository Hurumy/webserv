/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/26 11:17:52 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "Version.hpp"

Request::Request()
	: contentLength(0),
	  lastContentLength(contentLength),
	  phase(Request::REQLINE) {}

const std::string Request::getLines() const {
	std::string line;

	line += this->method;
	line += " ";
	line += this->url;
	line += " ";
	line += this->version;
	line += "\r\n";
	for (std::map<std::string, std::string>::const_iterator iter =
			 header.begin();
		 iter != header.end(); ++iter) {
		line += iter->first + ": " + iter->second + "\r\n";
	}
	line += "\r\n";
	line += this->body;
	return (line);
}

bool Request::setUrl(std::string _url) {
	this->url = _url;
	return (true);
}

std::string const &Request::getUrl() const { return (this->url); }

bool Request::setMethod(std::string _method) {
	this->method = _method;
	return (true);
}

std::string const &Request::getMethod() const { return (this->method); }

bool Request::loadPayload(CSocket &csocket) {
	bool isTrueLoadHeader;

	while (true) {
		switch (phase) {
			case Request::REQLINE:
				if (loadRequestLine(csocket) == true) {
					phase = Request::HEADER;
				} else {
					return false;
				}
				break;
			case Request::HEADER:
				isTrueLoadHeader = loadHeader(csocket);
				if (csocket.getData().compare(0, 2, "\r\n") == 0) {
					csocket.popDataLine();
					std::map<std::string, std::string>::iterator clengthiter =
						header.find("Content-Length");
					if (clengthiter != header.end()) {
						std::stringstream ss(clengthiter->second);
						ss >> contentLength;
						lastContentLength = contentLength;
					}
					phase = Request::BODY;
					break;
				}
				if (isTrueLoadHeader == false) {
					return false;
				}
				break;
			case Request::BODY: {
				std::string const csockData = csocket.getData();
				std::size_t beforeSize = body.size();
				body.append(csockData, 0, lastContentLength);
				std::size_t afterSize = body.size();
				csocket.eraseData(afterSize - beforeSize);
				lastContentLength -= afterSize - beforeSize;
				if (lastContentLength != 0) {
					csocket.setPhase(CSocket::RECV);
					return true;
				}
				phase = Request::REQLINE;
				csocket.setPhase(CSocket::PASS);
				// for debugging
				// std::clog << getLines() << std::endl;
			}
				return true;
		}
	}
	return true;
}

bool Request::loadRequestLine(CSocket &csocket) {
	std::istringstream iss(csocket.getDataLine());
	std::string _method;
	std::string _url;
	std::string _version;
	std::string extra;

	iss >> _method;
	iss >> _url;
	iss >> _version;
	iss >> extra;
	if (extra.empty() == false) {
		csocket.popDataLine();
		csocket.setPhase(CSocket::CLOSE);
		return false;
	}
	if (_method.empty() == true) {
		csocket.popDataLine();
		csocket.setPhase(CSocket::RECV);
		return false;
	}
	if (isValidURL(_url) == false) {
		// Should return error page
		csocket.setPhase(CSocket::CLOSE);
		return false;
	} else if (isVersion(_version) == false) {
		// Should return error page
		csocket.setPhase(CSocket::CLOSE);
		return false;
	}
	setMethod(_method);
	setUrl(_url);
	setVersion(_version);
	csocket.popDataLine();
	return true;
}

bool Request::loadHeader(CSocket &csocket) {
	std::istringstream iss(csocket.getDataLine());
	std::string key;
	std::string value;

	if (header.size() > header.max_size() - 1) {
		csocket.setPhase(CSocket::CLOSE);
		return false;
	}
	iss >> key;
	if (key.empty() == false) {
		key.resize(key.size() - 1);
	} else {
		csocket.setPhase(CSocket::RECV);
		return false;
	}
	iss >> value;
	if (value.empty() == false) {
		header[key] = value;
	} else {
		csocket.setPhase(CSocket::RECV);
		return false;
	}
	while (iss >> value) {
		header[key].append(" " + value);
	}
	csocket.popDataLine();
	return true;
}

bool Request::isMethod(std::string const &word) {
	return word.empty() == false;
}

bool Request::isValidURL(std::string const &word) {
	std::istringstream iss(word);
	std::string pathElem;
	long pos(0);

	if (word.empty() == true) {
		return false;
	}
	if (word[0] != '/') {
		return false;
	}
	while (iss.eof() == false) {
		std::getline(iss, pathElem, '/');
		if (pathElem.compare("..") == 0) {
			pos--;
		} else if (pathElem.empty() == false && pathElem.compare(".") != 0) {
			pos++;
		}
		if (0 > pos) {
			return false;
		}
	}
	return true;
}

bool Request::isVersion(std::string const &word) {
	if (word.empty() == true) {
		return false;
	}
	Result<std::string, bool> result = Version::getVersion(word);
	return result.isOK() == true;
}

void Request::setPhase(Request::tag _phase) { phase = _phase; }

const Request::tag &Request::getPhase() const { return phase; }

void Request::setRemoteAddr(std::string const &_remoteAddr) {
	remoteAddr = _remoteAddr;
}

std::string const &Request::getRemoteAddr() const { return remoteAddr; }

void Request::setLocalAddr(std::string const &_localAddr) {
	localAddr = _localAddr;
}

std::string const &Request::getLocalAddr() const { return localAddr; }

void Request::setLocalPort(unsigned int const _localPort) {
	localPort = _localPort;
}

unsigned int Request::getLocalPort() const { return localPort; }
