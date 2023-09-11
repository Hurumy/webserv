/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:53:37 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/12 01:16:11 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define BUFFER_SIZE 1000

#include <string>

#include "Result.hpp"

class CSocket {
	private:
		CSocket();

		int sockfd;
		short revents;
		bool isFirst;
	protected:
	public:
		CSocket(int const _sockfd);
		
		int getSockfd() const;
		short getRevents() const;
		void setRevents(short const _revents);
		bool getIsFirst() const;
		void setIsFirst(bool _isFirst);
		Result<std::string, bool>  getData() const;
		bool sendData(std::string const &data) const;
		bool closeSockfd() const;
};
