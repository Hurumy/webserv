/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 20:14:10 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "Version.hpp"

std::vector<std::string> const Request::methods = Request::initMethods();

std::vector<std::string> Request::initMethods() {
	std::vector<std::string> _methods;
	_methods.push_back("GET");
	_methods.push_back("HEAD");
	_methods.push_back("POST");
	_methods.push_back("PUT");
	_methods.push_back("DELETE");
	_methods.push_back("CONNECT");
	_methods.push_back("OPTIONS");
	_methods.push_back("TRACE");
	return _methods;
}

Request::Request()
	: contentLength(0), lastContentLength(contentLength), isCompleteHeader(false), phase(Request::REQLINE) {}

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

std::string const Request::getUrl(void) const { return (this->url); }

bool Request::setMethod(std::string _method) {
	this->method = _method;
	return (true);
}

std::string const Request::getMethod(void) const { return (this->method); }

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
					std::map<std::string, std::string>::iterator clengthiter = header.find("Content-Length");
					if (clengthiter != header.end()) {
						std::stringstream ss(clengthiter->second);
						ss >> contentLength;
						lastContentLength = contentLength;
					}
					phase = Request::BODY;
					break;
				}
				if (isTrueLoadHeader == false) {
					csocket.setPhase(CSocket::RECV);
					return false;
				}
				break;
			case Request::BODY:
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

	iss >> _method;
	iss >> _url;
	iss >> _version;
	if (_method.empty() == true) {
		csocket.popDataLine();
		csocket.setPhase(CSocket::RECV);
		return false;
	}
	if (isURL(_url) == false) {
		return false;
	}
	if (_version.empty() == true) {
		_version = "HTTP/1.1";
	}
	else if (isVersion(_version) == false) {
		// Should return error page 
		csocket.closeSockfd();
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

	iss >> key;
	if (key.empty() == false) {
		key.resize(key.size() - 1);
	} else {
		return false;
	}
	iss >> value;
	if (value.empty() == false) {
		header[key] = value;
	} else {
		return false;
	}
	while (iss >> value) {
		header[key].append(" " + value);
	}
	csocket.popDataLine();
	return true;
}

bool Request::isMethod(std::string const &word) {
	return std::find(methods.begin(), methods.begin(), word) != methods.end();
}

bool Request::isURL(std::string const &word) { return word.empty() == false; }

bool Request::isVersion(std::string const &word) {
	if (word.empty() == false) {
		return false;
	}
	Result<std::string, bool> result = Version::getVersion(word);
	return result.isOK() == true;
}

const Request::tag &Request::getPhase() const { return phase; }
