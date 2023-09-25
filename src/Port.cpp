/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:19:15 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/20 12:42:51 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Port.hpp"

#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

Result<Config, bool> Port::getConf(int _port) const {
	if (this->conf.empty() == true) {
		return Error<bool>(false);
	} else if (this->conf.find(_port) == this->conf.end()) {
		return Error<bool>(false);
	} else
		return Ok<Config>(this->conf.at(_port));
}

int Port::getHostPort() const { return (this->hostport); }

size_t Port::getNumofPorts() const { return (this->conf.size()); }

bool Port::addConf(int _port, Config &_conf) {
	this->conf[_port] = _conf;
	return (true);
}

bool Port::setHostPort(int _port) {
	this->hostport = _port;
	return (true);
}
