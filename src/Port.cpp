/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:19:15 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:47:34 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Port.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

Result<Config, bool>	Port::getConf(int port) const
{
	if (this->conf.empty() == true)
	{
		return Error<bool>(false);
	}
	else if (this->conf.find(port) == this->conf.end())
	{
		return Error<bool>(false);
	}
	else
		return Ok<Config>(this->conf.at(port));
}

int	Port::getHostPort() const
{
	return(this->hostport);
}

size_t	Port::getNumofPorts() const
{
	return (this->conf.size());
}

bool	Port::addConf(int port, Config &conf)
{
	this->conf[port] = conf;
	return (true);
}

bool	Port::addHostPort(int port)
{
	this->hostport = port;
	return (true);
}



