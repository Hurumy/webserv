/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 13:01:41 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/10 19:12:10 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CSocket.hpp"

#include <unistd.h>
#include <string>
#include <poll.h>

#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

CSocket::CSocket(int const _sockfd) : sockfd(_sockfd), revents(0) {}

int CSocket::getSockfd() const {
	return sockfd;
}

short CSocket::getRevents() const {
	return revents;
}

void CSocket::setRevents(short const _revents) {
	revents = _revents;
}

Result<std::string, bool> CSocket::getData() const {
	std::string data;
	char buff[BUFFER_SIZE];
	ssize_t readLen;

	while (true) {
		if ((revents & POLLIN) == 0) {
			break ;
		}
		std::memset(buff, 0, sizeof(char) * BUFFER_SIZE);
		readLen = read(sockfd, buff, BUFFER_SIZE);
		if (readLen == -1) {
			return Error<bool>(false);
		}
		if (readLen == 0) {
			break ;
		}
		data.append(buff, readLen);
	}
	return Ok<std::string>(data);
}
