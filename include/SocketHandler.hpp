/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:12:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/10 21:11:32 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>
#include <vector>
#include <map>

#include "Result.hpp"
#include "SSocket.hpp"
#include "CSocket.hpp"

class SocketHandler {
	private:
		SocketHandler();

		int timeout;
		std::vector<SSocket> ssockets;
		std::vector<CSocket> csockets;
		std::vector<struct pollfd> pollfds;
	protected:
	public:
		SocketHandler(std::vector<SSocket> &_ssockets, int const _timeout);

		bool initAllSSockets();
		bool closeAllSSockets();
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
		Result<std::map<int, std::string>, bool> getDataMap() const;
		bool sendDataMap(std::map<int, std::string> const &dataMap) const;
};
