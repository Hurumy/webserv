/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/18 13:54:08 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "SocketHandler.hpp"

static std::map<int, std::string> createResponse(std::vector<CSocket> const &csockets) {
	std::map<int, std::string> response;

	for (std::vector<CSocket>::const_iterator iter = csockets.begin(); iter != csockets.end(); ++iter) {
		if (iter->getPhase() == CSocket::PASS) {
			response[iter->getSockfd()] = std::string("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!");
		}
	}
	return response;
}

int main() {
	std::vector<SSocket> sources;
	std::map<int, std::string> request;
	std::map<int, std::string> response;

	sources.push_back(SSocket(8080, IPV4, 1000));
	sources.push_back(SSocket(8000, IPV4, 1000));
	SocketHandler socketHandler(sources, 10);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	while (true) {
		if (socketHandler.getCSockets().empty() == false) {
			socketHandler.recvCSocketsData();
			socketHandler.loadRequests();
			response = createResponse(socketHandler.getCSockets());
			socketHandler.sendDataMap(response);
		}
		socketHandler.recieveCSockets();
		socketHandler.clearPollfds();
		socketHandler.removeClosedCSockets();
		socketHandler.createPollfds();
		socketHandler.setRevents();
	}
	socketHandler.closeAllSSockets();
	return 0;
}
