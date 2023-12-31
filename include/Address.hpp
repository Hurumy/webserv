/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:10:30 by komatsud          #+#    #+#             */
/*   Updated: 2024/01/07 16:14:45 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "SSocket.hpp"
#include "webserv.hpp"

class Address {
	private:
		std::string ipaddress;
		int port;
		ipvers ipver;
		bool isaddress;

	protected:
	public:
		std::string const getIpAddress() const;
		int getPort() const;
		ipvers getIpVers() const;
		bool getIsAddress() const;
		bool setIpAddress(std::string address);
		bool setPort(int _port);
		bool setIpVers(int _protocol);
		Address();
		// ~Address();
		// Address(Address const &add);
};
