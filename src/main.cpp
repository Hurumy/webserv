/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/20 12:52:27 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "RequestHandler.hpp"
#include "Result.hpp"
#include "Server.hpp"
#include "SocketHandler.hpp"

#define CONF_FILE_PATH "./conf_files/test.conf"

int main(const int argc, const char **argv) {
	if (argc != 2) {
		return 1;
	}
	// std::vector<SSocket> sources;
	// std::map<int, std::string> responses;
	// Result<std::vector<Config>, bool> result =
	// parseConf(std::string(argv[1])); std::vector<Config> configs =
	// result.getOk();

	// Result<std::vector<Config>, bool> res =
	// parseConf("./conf_files/test.conf"); if (res.isError() == true) {
	// 	std::cout << "parsing Configs failed" << std::endl;
	// 	return (-1);
	// }
	// std::vector<Config> conf = res.getOk();

	// sources.push_back(SSocket(8080, IPV4, 1000));
	// sources.push_back(SSocket(8000, IPV4, 1000));
	// SocketHandler socketHandler(sources, 60, 30);
	// socketHandler.initAllSSockets();
	// socketHandler.createPollfds();
	// socketHandler.setRevents();
	// while (true) {
	// 	// for siege command
	// 	// usleep(10000);
	// 	if (socketHandler.getCSockets().empty() == false) {
	// 		socketHandler.recvCSocketsData();
	// 		socketHandler.loadRequests();
	// 		// for developing CGI
	// 		socketHandler.handleCGIRequest();
	// 		// responses = socketHandler.createResponse();
	// 		// socketHandler.loadResponses(configs);
	// 		// socketHandler.sendDataMap(responses);
	// 		socketHandler.sendResponses();
	// 	}
	// 	socketHandler.recieveCSockets();
	// 	socketHandler.clearPollfds();
	// 	socketHandler.createPollfds();
	// 	socketHandler.setRevents();
	// 	socketHandler.closeTimeoutCSockets();
	// 	socketHandler.removeClosedCSockets();
	// }
	// socketHandler.closeAllSSockets();

	Server server;

	if (server.startUp(std::string(argv[1])) == false) return 1;
	if (server.serverLoop() == false) {
		return 1;
	}
	if (server.down() == false) {
		return 1;
	}
	return 0;
}
