/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/17 20:24:17 by shtanemu         ###   ########.fr       */
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
	  isCompleteHeader(false),
	  phase(Request::REQLINE),
	  monitoredfd(0),
	  revents(0) {
	std::memset(inpfd, 0, sizeof(inpfd));
	std::memset(outpfd, 0, sizeof(outpfd));
}

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
			case Request::CGISTARTUP:
				return false;
			case Request::CGIWRITE:
				return false;
			case Request::CGIRECV:
				return false;
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

void Request::setMonitoredfd(Request::tag _phase) {
	switch (_phase) {
		case Request::CGIWRITE:
			monitoredfd = inpfd[1];
			break;
		case Request::CGIRECV:
			monitoredfd = outpfd[0];
			break;
		default:
			break;
	}
}

int Request::getMonitoredfd() const { return monitoredfd; }

short Request::getRevents() const { return revents; }

void Request::setRevents(short const _revents) { revents = _revents; }

bool Request::setEnvVars() const { return true; }

bool Request::execCGIScript() {
	extern char **environ;

	if (pipe(inpfd) == -1) {
		// error handling
	}
	if (pipe(outpfd) == -1) {
		// error handling
	}
	pid_t const pid = fork();
	if (pid == -1) {
		// error handling
	}
	if (pid == 0) {
		// for developement
		char *const argv[] = {strdup("/bin/ls"), NULL};
		dup2(inpfd[0], 0);
		dup2(outpfd[1], 1);
		close(inpfd[0]);
		close(inpfd[1]);
		close(outpfd[0]);
		close(outpfd[1]);
		execve("/bin/ls", argv, environ);
		std::exit(EXIT_FAILURE);
	}
	return true;
}

bool Request::writeMessageBody() const {
	// for develope
	if ((revents & POLLOUT) != POLLOUT) {
		return false;
	}
	write(inpfd[1], "Makefile", 8);
	return true;
}

bool Request::recvCGIOutput() const {
	ssize_t readLen;
	char buf[BUFFER_SIZE + 1] = {0};

	if ((revents & POLLIN) != POLLIN) {
		return false;
	}
	readLen = read(outpfd[0], buf, BUFFER_SIZE);
	if (readLen == -1) {
		// errorhandling
		return false;
	}
	// for develope
	buf[BUFFER_SIZE] = '\0';
	std::clog << "buf: " << buf << std::endl;
	return true;
}

void Request::setRemoteAddr(std::string const &_remoteAddr) {
	remoteAddr = _remoteAddr;
}

std::string const &Request::getRemoteAddr() const { return remoteAddr; }
