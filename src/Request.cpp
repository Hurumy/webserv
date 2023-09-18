/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 15:17:47 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

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

Request::Request() : contentLength(0), isCompleteHeader(false), phase(Request::REQLINE) {}

const std::string Request::getLines() const {
	std::string line;

	line += this->method;
	line += " ";
	line += this->url;
	line += " ";
	line += this->version;
	line += "\n";
	for (std::map<std::string, std::string>::const_iterator iter = header.begin(); iter != header.end(); ++iter) {
		line += iter->first + ": " + iter->second + "\n";
	}
	line += this->body;
	return (line);
}

bool Request::setUrl(std::string url) {
	this->url = url;
	return (true);
}

std::string const Request::getUrl(void) const { return (this->url); }

bool Request::setMethod(std::string method) {
	this->method = method;
	return (true);
}

std::string const Request::getMethod(void) const { return (this->method); }

bool Request::loadPayload(CSocket &csocket) {
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
				if (loadHeader(csocket) == true) {
					if (csocket._getData().compare(0, 2, "\r\n") == 0) {
						std::map<std::string, std::string>::iterator clengthiter = header.find("Content-Length");
						if (clengthiter != header.end()) {
							std::stringstream ss(clengthiter->second);
							ss >> contentLength;
							return true;
						} else {
							// error handling
						}
						phase = Request::BODY;
					}
				} else {
					return false;
				}
				break;
			case Request::BODY:
				body.append(csocket._getData(), contentLength);
				csocket.eraseData(contentLength);
				phase = Request::REQLINE;
				csocket.setPhase(CSocket::PASS);
				std::clog << getLines() << std::endl;
				return true;
				break;
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
	if (isMethod(_method) == false) {
		return false;
	}
	if (isURL(_url) == false) {
		return false;
	}
	if (isVersion(_version) == false) {
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

// bool Request::loadPayload(CSocket &csocket) {
// 	std::string line;

// 	// should check about content-length
// 	if ((csocket._getData().find("\r\n") == std::string::npos) && (url.empty() || method.empty() || version.empty() || isCompleteHeader == false)) {
// 		return false;
// 	}
// 	while (csocket._getData().empty() == false && contentLength >= body.size()) {
// 		if ((csocket._getData().find("\r\n") == std::string::npos) && (url.empty() || method.empty() || version.empty() || isCompleteHeader == false)) {
// 			return false;
// 		}
// 		line = csocket.getDataLine();
// 		if (url.empty() || method.empty() || version.empty()) {
// 			if (setRequestLine(line) == false) {	
// 				// error handling
// 			} else {
// 				csocket.popDataLine();
// 			}
// 		} else if (isCompleteHeader == false) {
// 			if (setHeader(line) == false) {
// 				// error handling
// 			} else {
// 				csocket.popDataLine();
// 			}
// 		} else if (contentLength > body.size()) {
// 			body.append(line);
// 			csocket.popDataLine();
// 		}
// 	}
// 	return true;
// }

// bool Request::loadHeader(CSocket &csocket) {
// 	std::string line;

// 	// should check about content-length
// 	if ((csocket._getData().find("\r\n") == std::string::npos) && (url.empty() || method.empty() || version.empty() || isCompleteHeader == false)) {
// 		return false;
// 	}
// 	while (csocket._getData().empty() == false && contentLength >= body.size()) {
// 		if ((csocket._getData().find("\r\n") == std::string::npos) && (url.empty() || method.empty() || version.empty() || isCompleteHeader == false)) {
// 			return false;
// 		}
// 		line = csocket.getDataLine();
// 		if (url.empty() || method.empty() || version.empty()) {
// 			if (setRequestLine(line) == false) {	
// 				// error handling
// 			} else {
// 				csocket.popDataLine();
// 			}
// 		} else if (isCompleteHeader == false) {
// 			if (setHeader(line) == false) {
// 				// error handling
// 			} else {
// 				csocket.popDataLine();
// 			}
// 		} else if (contentLength > body.size()) {
// 			body.append(line);
// 			csocket.popDataLine();
// 		}
// 	}
// 	return true;
// }

bool Request::isMethod(std::string const &word) {
	return std::find(methods.begin(), methods.begin(), word) != methods.end();
}

bool Request::isURL(std::string const &word) {
	return word.empty() == false;
}

bool Request::isVersion(std::string const &word) {
	return word.empty() == false;
}

bool Request::setRequestLine(std::string const &line) {
	std::istringstream iss(line);
	std::string _method;
	std::string _url;
	std::string _version;

	iss >> _method;
	iss >> _url;
	iss >> _version;
	if (isMethod(_method) == false) {
		return false;
	}
	if (isURL(_url) == false) {
		return false;
	}
	if (isVersion(_version) == false) {
		return false;	
	}
	setMethod(_method);
	setUrl(_url);
	setVersion(_version);
	return true;
}

bool Request::setHeader(std::string const &line) {
	std::istringstream iss(line);
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
	return true;
}

const Request::tag &Request::getPhase() const {
	return phase;
}

// bool Request::setHeader(std::string const &line) {
// 	std::istringstream iss(line);
// 	std::string key;
// 	std::string value;

// 	iss >> key;
// 	iss >> value;
// 	if ((key.empty() == false) && (value.empty() == false)) {
// 		key.resize(key.size() - 1);
// 		header[key] = value;
// 	} else {
// 		return false;
// 	}
// 	return true;
// }

// test code
/*
int main()
{
	Request *test = new Request();

	test->setversion("v1.2\n");
	test->seturl("http:\n");
	test->setbody("NICE BODY~\n");

	std::cout << test->getLines() << std::endl;
	std::cout << test->getUrl() << std::endl;
	std::cout << test->getVersion() << std::endl;
	std::cout << test->getBody() << std::endl;

	delete test;
	return (0);
}*/
