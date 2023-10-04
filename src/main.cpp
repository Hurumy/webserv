/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/03 19:49:14 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "SocketHandler.hpp"
#include "RequestHandler.hpp"

int main() {
	std::vector<SSocket> sources;
	std::map<int, std::string> request;
	std::map<int, std::string> response;

	Result<std::vector<Config>, bool> res = parseConf("./conf_files/test.conf");
	if (res.isError() == true)
	{
		std::cout << "parsing Configs failed" << std::endl;
		return (-1);
	}
	std::vector<Config> conf = res.getOk();

	sources.push_back(SSocket(8080, IPV4, 1000));
	sources.push_back(SSocket(8000, IPV4, 1000));
	SocketHandler socketHandler(sources, 10);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	while (true) {
		// for siege command
		// usleep(10000);
		if (socketHandler.getCSockets().empty() == false) {
			socketHandler.recvCSocketsData();
			socketHandler.loadRequests();
			response = socketHandler.createResponse();
			socketHandler.sendDataMap(response);
		}
		socketHandler.recieveCSockets();
		socketHandler.clearPollfds();
		socketHandler.createPollfds();
		socketHandler.setRevents();
		socketHandler.removeClosedCSockets();
	}
	socketHandler.closeAllSSockets();

	return 0;
}
