/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:53:37 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/17 20:17:32 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define BUFFER_SIZE 1000

#include <ctime>
#include <string>

#include "Result.hpp"

class CSocket {
	protected:
	public:
		CSocket(int const _sockfd);
		CSocket(int const _sockfd, unsigned long s_addr);
		enum tag {
				RECV,
				LOAD,
				PASS,
				CGI,
				SEND,
				CLOSE,
		};

		int getSockfd() const;
		short getRevents() const;
		void setRevents(short const _revents);
		bool readData();
		std::string const &getData() const;
		void setData(std::string const &_data);
		std::string popDataLine();
		std::string getDataLine() const;
		bool sendData(std::string const &_data) const;
		bool closeSockfd() const;
		void setPhase(CSocket::tag _phase);
		CSocket::tag getPhase() const;
		bool eraseData(std::size_t until);
		std::time_t const &getLasttime() const;
		void setLasttime(std::time_t const &_lasttime);
		void setRemoteAddr(unsigned long s_addr);
		std::string const &getRemoteAddr() const;

	private:
		CSocket();

		int sockfd;
		short revents;
		std::string data;
		tag phase;
		std::time_t lasttime;
		std::string remoteAddr;
};
