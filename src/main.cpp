/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/02 15:45:33 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "SocketHandler.hpp"

int main() {
	std::vector<SSocket> sources;
	std::map<int, std::string> responses;
	
	sources.push_back(SSocket(8080, IPV4, 1000));
	sources.push_back(SSocket(8000, IPV4, 1000));
	SocketHandler socketHandler(sources, 10, 10);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	while (true) {
		// for siege command
		// usleep(10000);
		if (socketHandler.getCSockets().empty() == false) {
			socketHandler.recvCSocketsData();
			socketHandler.loadRequests();
			responses = socketHandler.createResponse();
			socketHandler.sendDataMap(responses);
		}
		socketHandler.recieveCSockets();
		socketHandler.clearPollfds();
		socketHandler.createPollfds();
		socketHandler.setRevents();
		socketHandler.closeTimeoutCSockets();
		socketHandler.removeClosedCSockets();
	}
	socketHandler.closeAllSSockets();

	return 0;
}
