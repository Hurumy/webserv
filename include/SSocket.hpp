/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:15:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 12:02:12 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

enum ipvers {
	IPV4,
	IPV6,
};

class SSocket {
	private:
		SSocket();

		int sockfd;
		int port;
		ipvers ipver;
		int backlog;
	protected:
	public:
		SSocket(int _port, ipvers _ipver, int _backlog);

		bool init();
		int const &getPort() const;
		int const &getSockfd() const;
		ipvers const &getIpVer() const;
		int const &getBacklog() const;
};
