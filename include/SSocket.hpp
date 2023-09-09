/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:15:38 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 15:08:41 by shtanemu         ###   ########.fr       */
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
		short revents;
	protected:
	public:
		SSocket(int _port, ipvers _ipver, int _backlog);

		bool init();
		int const &getPort() const;
		int const &getSockfd() const;
		ipvers const &getIpVer() const;
		int const &getBacklog() const;
		short getRevents() const;
		void setRevents(short const _revents);
};
