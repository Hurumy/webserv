/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/03 21:57:41 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "SocketHandler.hpp"

#define CONF_FILE_PATH "./conf_files/test.conf"

int main(const int argc, const char **argv) {
	if (argc != 2) {
		return 1;
	}
	std::vector<SSocket> sources;
	std::map<int, std::string> responses;
	Result<std::vector<Config>, bool> result = parseConf(std::string(argv[1]));
	std::vector<Config> configs = result.getOk();

	sources.push_back(SSocket(8080, IPV4, 1000));
	sources.push_back(SSocket(8000, IPV4, 1000));
	SocketHandler socketHandler(sources, 10, 30);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	while (true) {
		// for siege command
		// usleep(10000);
		if (socketHandler.getCSockets().empty() == false) {
			socketHandler.recvCSocketsData();
			socketHandler.loadRequests();
			// responses = socketHandler.createResponse();
			socketHandler.loadResponses(configs);
			// socketHandler.sendDataMap(responses);
			socketHandler.sendResponses();
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
