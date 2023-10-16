/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:15:55 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 19:45:38 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "webserv.hpp"

class Port {
   private:
	std::map<int, Config> conf;
	int hostport;

   protected:
   public:
	Result<Config, bool> getConf(int port) const;
	int getHostPort() const;
	size_t getNumofPorts() const;

	// for test
	bool addConf(int port, Config &_conf);
	bool setHostPort(int port);
};
