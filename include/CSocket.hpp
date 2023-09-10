/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:53:37 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/10 17:44:27 by shtanemu         ###   ########.fr       */
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
	protected:
	public:
		CSocket(int const _sockfd);
		
		int getSockfd() const;
		short getRevents() const;
		void setRevents(short const _revents);
		Result<std::string, bool>  getData() const;
};
