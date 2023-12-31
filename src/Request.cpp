/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/12/28 15:24:57 by shtanemu         ###   ########.fr       */
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
	  phase(Request::REQLINE),
	  cntCGIExec(0),
	  chunkLength(0),
	  cntRemovedTrailers(0) {}

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
						header.find("content-length");
					if (clengthiter != header.end()) {
						std::stringstream ss(clengthiter->second);
						ss >> contentLength;
						lastContentLength = contentLength;
					}
					phase = Request::BODY;
					std::map<std::string, std::string>::iterator cencodeiter =
						header.find("transfer-encoding");
					if (cencodeiter != header.end()) {
						std::istringstream iss(cencodeiter->second);
						std::string element;

						while (true) {
							if (iss.eof() == true) {
								break;
							}
							std::getline(iss, element, ',');
							std::stringstream sselem(element);
							std::string encoding;
							sselem >> encoding;
							if (encoding.compare("chunked") == 0) {
								phase = Request::CHUNKEDBODY;
							}
						}
					}
					std::map<std::string, std::string>::iterator expectiter =
						header.find("expect");
					if (expectiter != header.end()) {
						std::istringstream iss(expectiter->second);
						std::string element;

						while (true) {
							if (iss.eof() == true) {
								break;
							}
							std::getline(iss, element, ',');
							std::stringstream sselem(element);
							std::string expect;
							sselem >> expect;
							if (expect.compare("100-continue") == 0) {
								csocket.setPhase(CSocket::SETCONTINUE);
								return true;
							}
						}
					}
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
					return false;
				}
				phase = Request::REQLINE;
				csocket.setPhase(CSocket::PASS);
			}
				return true;
			case Request::CHUNKEDBODY: {
				std::string chunkLine(csocket.getDataLine());
				std::stringstream ss;
				std::size_t chunkSize(0);

				ss << chunkLine;
				ss >> std::hex >> chunkSize;
				ss >> chunkExt;
				while (chunkSize > 0) {
					if (csocket.getData().size() < chunkSize) {
						csocket.setPhase(CSocket::RECV);
						return false;
					}
					csocket.popDataLine();
					body.append(csocket.getData(), 0, chunkSize);
					csocket.eraseData(chunkSize + 2);
					chunkLength += chunkSize;
					chunkSize = 0;
					chunkLine = csocket.getDataLine();
					if (chunkLine.empty() == true) {
						csocket.setPhase(CSocket::RECV);
						return false;
					}
					ss.str("");
					ss.clear(std::stringstream::goodbit);
					ss << chunkLine;
					ss >> std::hex >> chunkSize;
					ss >> chunkExt;
				}
				std::stringstream ssChunkLength;
				ssChunkLength << chunkLength;
				header["Content-Length"] = ssChunkLength.str();
				std::map<std::string, std::string>::iterator headerIter =
					header.find("trailer");
				if (headerIter != header.end()) {
					csocket.popDataLine();
					phase = Request::TRAILERFIELD;
					break;
				}
				csocket.setPhase(CSocket::PASS);
			}
				return true;
			case Request::TRAILERFIELD: {
				if (trailers.empty() == true) {
					std::map<std::string, std::string>::iterator headerIter =
						header.find("trailer");
					if (headerIter == header.end()) {
						csocket.setPhase(CSocket::PASS);
						return true;
					}

					std::stringstream ss(headerIter->second);
					std::stringstream ssForRemoveSpaces;
					std::string trailer;

					while (ss.good() == true) {
						std::getline(ss, trailer, ',');
						ssForRemoveSpaces.str("");
						ssForRemoveSpaces.clear(std::stringstream::goodbit);
						ssForRemoveSpaces << trailer;
						trailer.clear();
						ssForRemoveSpaces >> trailer;
						trailers[trailer] = "";
					}
				}

				std::stringstream ss;
				std::string key;
				std::string value;

				while (true) {
					if (csocket.getDataLine().empty() == true &&
						trailers.size() == cntRemovedTrailers) {
						csocket.popDataLine();
						csocket.setPhase(CSocket::PASS);
						return true;
					}
					ss.str("");
					ss.clear(std::stringstream::goodbit);
					ss << csocket.getDataLine();
					std::getline(ss, key, ':');
					if (ss.good() == false) {
						csocket.setPhase(CSocket::RECV);
						return false;
					}
					std::map<std::string, std::string>::iterator trailersIter =
						trailers.find(key);
					if (trailersIter == trailers.end() &&
						trailers.size() == cntRemovedTrailers) {
						csocket.setPhase(CSocket::PASS);
						return true;
					} else if (trailersIter == trailers.end()) {
						csocket.setPhase(CSocket::RECV);
						return false;
					}
					std::getline(ss, value, ':');
					setHeader(key, value);
					csocket.popDataLine();
					cntRemovedTrailers++;
				}
			}
				return true;
		}
	}
	csocket.setPhase(CSocket::PASS);
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
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	}
	if (_method.empty() == true) {
		csocket.popDataLine();
		csocket.setPhase(CSocket::RECV);
		return false;
	}
	if (isValidURL(_url) == false) {
		// Should return error page
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	} else if (isVersion(_version) == false) {
		// Should return error page
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	}
	setMethod(_method);
	setUrl(_url);
	setVersion(_version);
	csocket.popDataLine();
	return true;
}

bool Request::loadHeader(CSocket &csocket) {
	if (csocket.getData().find("\r\n") == std::string::npos) {
		csocket.setPhase(CSocket::RECV);
		return false;
	}

	std::istringstream iss(csocket.getDataLine());
	std::string key;
	std::string value;

	if (header.size() > header.max_size() - 1) {
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	}
	if (iss.good() == false) {
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	}
	std::getline(iss, key, ':');
	if (iss.good() == false) {
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	}
	iss >> value;
	if (value.empty() == true) {
		csocket.setPhase(CSocket::CSETERROR);
		return false;
	}
	header[key] = value;
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

void Request::countUpCntCGIExec() { cntCGIExec++; }

std::size_t Request::getCntCGIExec() const { return cntCGIExec; }