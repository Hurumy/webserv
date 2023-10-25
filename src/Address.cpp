/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:24:50 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/18 14:46:22 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Address.hpp"

Address::Address() : isaddress(false) {}

std::string const Address::getIpAddress() const { return (this->ipaddress); }

int Address::getPort() const { return (this->port); }

int Address::getIpVers() const { return (this->ipver); }

bool Address::getIsAddress() const { return (this->isaddress); }

bool Address::setIpAddress(std::string address) {
	this->isaddress = true;
	this->ipaddress = address;
	return (true);
}

bool Address::setPort(int _port) {
	this->port = _port;
	return (true);
}

bool Address::setIpVers(int _protocol) {
	if (_protocol == IPV4) {
		this->ipver = IPV4;
		return (true);
	} else if (_protocol == IPV6) {
		this->ipver = IPV6;
		return (true);
	} else {
		return (false);
	}
}
