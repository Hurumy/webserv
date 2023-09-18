/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 13:01:41 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/18 15:27:30 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CSocket.hpp"

#include <unistd.h>
#include <string>
#include <cstring>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <errno.h>

#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

CSocket::CSocket(int const _sockfd) : sockfd(_sockfd), revents(0), isFirst(true), phase(CSocket::RECV) {}

int CSocket::getSockfd() const {
	return sockfd;
}

short CSocket::getRevents() const {
	return revents;
}

void CSocket::setRevents(short const _revents) {
	revents = _revents;
}

bool CSocket::getIsFirst() const {
	return isFirst;
}

void CSocket::setIsFirst(bool _isFirst) {
	isFirst = _isFirst;
}

bool CSocket::sendData(std::string const &data) const{
	if (write(sockfd, data.c_str(), data.size()) == -1) {
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
	data.append(buff, readLen);
	return true;
}

std::string const &CSocket::getData() const {
	return data;
}

void CSocket::setData(std::string const &_data) {
	data = _data;
}

std::string CSocket::popDataLine() {
	std::istringstream iss(data);
	std::string line;
	
	std::getline(iss, line);
	data.erase(0, data.find("\n") + 1);
	return line;
}

std::string CSocket::getDataLine() const {
	std::istringstream iss(data);
	std::string line;
	
	std::getline(iss, line);
	return line;
}

void CSocket::setPhase(CSocket::tag _phase) {
	phase = _phase;
}

CSocket::tag CSocket::getPhase() const{
	return phase;
}

bool CSocket::eraseData(std::size_t until) {
	data.erase(0, until);
	return true;
}
