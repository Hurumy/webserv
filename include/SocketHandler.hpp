/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:12:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/12/25 10:57:34 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>

#include <cstring>
#include <list>
#include <map>
#include <vector>

#include "AddrMonitor.hpp"
#include "CGIResponseCreator.hpp"
#include "CSocket.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "SSocket.hpp"

class SocketHandler {
	private:
		std::list<CSocket>::iterator _deinitCSocket(
			std::list<CSocket>::iterator &csockiter);

		std::vector<SSocket> ssockets;
		std::list<CSocket> csockets;
		std::list<pid_t> cpids;
		std::size_t timeout;
		int pollTimeout;
		std::vector<struct pollfd> pollfds;
		std::map<int, Request> requests;
		std::map<int, Response> responses;
		std::map<int, CGIResponseCreator> cgiResponseCreators;
		AddrMonitor addrMonitor;

	protected:
	public:
		SocketHandler();
		SocketHandler(std::vector<SSocket> &_ssockets,
					  std::size_t const _timeout, int _pollTimeout);

		bool initAllSSockets();
		bool closeAllSSockets();
		bool removeRequest(int const csockfd);
		bool removeResponse(int const csockfd);
		bool removeClosedCSockets();
		std::vector<SSocket> const &getSSockets() const;
		void setSSockets(const std::vector<SSocket> &_ssockets);
		std::list<CSocket> const &getCSockets() const;
		int getTimeout() const;
		void setTimeout(const std::size_t _timeout);
		void setPollTimeout(const int _pollTimeout);
		void addCSocket(CSocket const &_csocket);
		bool createPollfds();
		void clearPollfds();
		std::vector<struct pollfd> const &getPollfds() const;
		bool setRevents();
		bool recieveCSockets();
		std::map<int, Request> const &getRequests() const;
		bool recvCSocketsData();
		bool sendResponses();
		bool loadRequests();
		bool loadResponses(std::vector<Config> const &configs);
		bool handleCGIRequest(std::vector<Config> const &configs);
		bool closeTimeoutCSockets();
		bool waitDeadCGIProcs();
};
