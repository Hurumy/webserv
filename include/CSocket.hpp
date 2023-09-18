/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:53:37 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/18 15:27:30 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define BUFFER_SIZE 1000

#include <string>

#include "Result.hpp"

class CSocket {
	protected:
	public:
		CSocket(int const _sockfd);
		enum tag {
			RECV,
			LOAD,
			PASS,
			SEND,
			CLOSE,
		};
		
		int getSockfd() const;
		short getRevents() const;
		void setRevents(short const _revents);
		bool getIsFirst() const;
		void setIsFirst(bool _isFirst);
		bool readData();
		std::string const &getData() const;
		void setData(std::string const &_data);
		std::string popDataLine();
		std::string getDataLine() const;
		bool sendData(std::string const &data) const;
		bool closeSockfd() const;
		void setPhase(CSocket::tag _phase);
		CSocket::tag getPhase() const;
		bool eraseData(std::size_t until);
	private:
		CSocket();

		int sockfd;
		short revents;
		bool isFirst;
		std::string data;
		tag phase;
};
