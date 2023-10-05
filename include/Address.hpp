/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:10:30 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 16:28:59 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Address {
   private:
	std::string ipaddress;
	int port;
	int ipvers;

   protected:
   public:
	std::string const getIpAddress() const;
	int getPort() const;
	int getIpVers() const;
	bool setIpAddress(std::string address);
	bool setPort(int _port);
	bool setIpVers(int _protocol);
	Address();
	Address(Address const &add);
};
