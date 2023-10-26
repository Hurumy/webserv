/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 13:01:41 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/26 10:54:09 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CSocket.hpp"

#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <sstream>
#include <string>

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

CSocket::CSocket(int const _sockfd)
	: sockfd(_sockfd),
	  revents(0),
	  phase(CSocket::RECV),
	  lasttime(std::time(NULL)) {}

CSocket::CSocket(int const _sockfd, unsigned long s_addr, std::string const &_localAddr)
	: sockfd(_sockfd),
	  revents(0),
	  phase(CSocket::RECV),
	  lasttime(std::time(NULL)),
	  localAddr(_localAddr) {
	setRemoteAddr(s_addr);
}

int CSocket::getSockfd() const { return sockfd; }

short CSocket::getRevents() const { return revents; }

void CSocket::setRevents(short const _revents) { revents = _revents; }

bool CSocket::sendData(std::string const &_data) const {
	if (write(sockfd, _data.c_str(), _data.size()) == -1) {
		return false;
	}
	return true;
}

bool CSocket::closeSockfd() const {
	if (close(sockfd) == -1) {
		return false;
	}
	return true;
}

bool CSocket::readData() {
	char buff[BUFFER_SIZE] = {0};
	ssize_t readLen;

	if ((revents & POLLIN) != POLLIN) {
		return false;
	}
	readLen = recv(sockfd, buff, BUFFER_SIZE, 0);
	if (readLen == -1) {
		return false;
	}
	if (data.size() > data.max_size() - readLen) {
		phase = CSocket::CLOSE;
		return false;
	}
	data.append(buff, readLen);
	return true;
}

std::string const &CSocket::getData() const { return data; }

void CSocket::setData(std::string const &_data) { data = _data; }

std::string CSocket::popDataLine() {
	std::istringstream iss(data);
	std::string line;

	std::getline(iss, line);
	data.erase(0, data.find("\r\n") + 2);
	return line;
}

std::string CSocket::getDataLine() const {
	std::istringstream iss(data);
	std::string line;

	std::getline(iss, line);
	return line;
}

void CSocket::setPhase(CSocket::tag _phase) { phase = _phase; }

CSocket::tag CSocket::getPhase() const { return phase; }

bool CSocket::eraseData(std::size_t until) {
	data.erase(0, until);
	return true;
}

std::time_t const &CSocket::getLasttime() const { return lasttime; }

void CSocket::setLasttime(std::time_t const &_lasttime) {
	lasttime = _lasttime;
}

void CSocket::setRemoteAddr(u_int32_t s_addr) {
	int byte;
	int bitshift(32);
	std::stringstream ss;

	while (bitshift != 0) {
		bitshift = bitshift - 8;
		byte = (s_addr >> bitshift) & 0xFF;
		ss << byte;
		if (bitshift == 0) {
			remoteAddr.append(ss.str());
		} else {
			remoteAddr.append(ss.str() + ".");
		}
		ss.str("");
		ss.clear(std::stringstream::goodbit);
	}
}

std::string const &CSocket::getRemoteAddr() const { return remoteAddr; }

std::string const &CSocket::getLocalAddr() const { return localAddr; }
