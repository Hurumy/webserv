/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/12 00:04:43 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "SocketHandler.hpp"

static std::map<int, std::string> createResponse(std::map<int, std::string> const &request) {
	std::map<int, std::string> response;
	
	for (std::map<int, std::string>::const_iterator iter = request.begin(); iter != request.end(); ++iter) {
		response[iter->first] = std::string("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
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
			Result<std::map<int, std::string>, bool> dataMap = socketHandler.getDataMap();
			if (dataMap.isOK() == true) {
				request = dataMap.getOk();
				response = createResponse(request);
				socketHandler.sendDataMap(response);
			}
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
