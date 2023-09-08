/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Port.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:15:55 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:32:29 by komatsud         ###   ########.fr       */
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
	protected:

	public:
		Result<Config, bool>		getConf(int port) const;

		//for test
		bool						addConf(int port, Config &conf);
};

