/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:24:50 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 15:34:02 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Address.hpp"

Address::Address()
{
	this->ipaddress = "";
	this->port = 0;
	this->ipvers = -1;
}

Address::Address(Address &add)
{
	this->ipaddress = add.getIpAddress();
	this->port = add.getPort();
	this->ipvers = add.getIpVers();
}

std::string const	Address::getIpAddress() const
{
	return (this->ipaddress);
}

int		Address::getPort() const
{
	return (this->port);
}

int		Address::getIpVers() const
{
	return (this->ipvers);
}

bool	Address::setIpAddress(std::string address)
{
	this->ipaddress = address;
	return (true);
}

bool	Address::setPort(int port)
{
	this->port = port;
	return (true);
}

bool	Address::setIpVers(int protocol)
{
	this->ipvers = protocol;
	return (true);
}
