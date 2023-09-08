/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:15:55 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:55:23 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "Config.hpp"

class	Port
{
	private:
		std::map<int, Config>			conf;
		int								hostport;
	
	protected:

	public:
		Result<Config, bool>		getConf(int port) const;
		int							getHostPort() const;
		size_t						getNumofPorts() const;

		//for test
		bool						addConf(int port, Config &conf);
		bool						setHostPort(int port);
};

