/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:12:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/13 22:27:48 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>

#include <cstring>
#include <map>
#include <vector>

#include "CSocket.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "SSocket.hpp"
#include "CGIResponseCreator.hpp"

class SocketHandler {
   private:
	SocketHandler();

	std::vector<SSocket> ssockets;
	std::vector<CSocket> csockets;
	std::size_t timeout;
	int pollTimeout;
	std::vector<struct pollfd> pollfds;
	std::map<int, Request> requests;
	std::map<int, Response> responses;
	std::map<int, CGIResponseCreator> cgiResponseCreators;
	bool isCGI;

   protected:
   public:
	SocketHandler(std::vector<SSocket> &_ssockets, std::size_t const _timeout,
				  int _pollTimeout);

	bool initAllSSockets();
	bool closeAllSSockets();
	bool removeRequest(int const csockfd);
	bool removeResponse(int const csockfd);
	bool removeClosedCSockets();
	std::vector<SSocket> const &getSSockets() const;
	std::vector<CSocket> const &getCSockets() const;
	int getTimeout() const;
	void addCSocket(CSocket const &_csocket);
	bool createPollfds();
	void clearPollfds();
	std::vector<struct pollfd> const &getPollfds() const;
	bool setRevents();
	bool recieveCSockets();
	std::map<int, Request> const &getRequests() const;
	bool recvCSocketsData();
	bool sendDataMap(std::map<int, std::string> const &dataMap);
	bool sendResponses();
	bool loadRequests();
	std::map<int, std::string> createResponse();
	bool loadResponses(std::vector<Config> const &configs);
	bool handleCGIRequest();
	bool closeTimeoutCSockets();
};
