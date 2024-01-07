/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:16:29 by shtanemu          #+#    #+#             */
/*   Updated: 2024/01/07 16:05:03 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <signal.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "ft.hpp"

bool Server::startUp(std::string const &pathConfig) {
	Result<std::vector<Config>, bool> result(parseConf(pathConfig));

	if (result.isError() == true) {
		return false;
	}
	configs = result.getOk();

	for (std::vector<Config>::iterator confiter = configs.begin();
		 confiter != configs.end(); ++confiter) {
		std::vector<Address> const &addresses(confiter->getAddresses());
		for (std::vector<Address>::const_iterator addriter = addresses.begin();
			 addriter != addresses.end(); ++addriter) {
			sources.push_back(SSocket(addriter->getIpAddress(),
									  addriter->getPort(), addriter->getIpVers(), 1000));
		}
	}
	socketHandler.setSSockets(sources);
	socketHandler.setTimeout(60);
	socketHandler.setPollTimeout(10);
	if (socketHandler.initAllSSockets() == false) return false;
	if (socketHandler.createPollfds() == false) return false;
	if (socketHandler.setRevents() == false) return false;
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		ft::putSystemError("signal");
		return false;
	}
	return true;
}

bool Server::down() { return socketHandler.closeAllSSockets(); }

bool Server::serverLoop() {
	while (true) {
		if (socketHandler.getCSockets().empty() == false) {
			socketHandler.recvCSocketsData();
			socketHandler.loadRequests();
			socketHandler.handleCGIRequest(configs);
			socketHandler.loadResponses(configs);
			socketHandler.sendResponses();
		}
		socketHandler.recieveCSockets();
		socketHandler.clearPollfds();
		socketHandler.createPollfds();
		socketHandler.setRevents();
		socketHandler.closeTimeoutCSockets();
		socketHandler.removeClosedCSockets();
		socketHandler.waitDeadCGIProcs();
	}
	return true;
}
