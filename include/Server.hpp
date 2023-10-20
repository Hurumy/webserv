/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:01:32 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/20 12:45:24 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "SocketHandler.hpp"
#include "SSocket.hpp"
#include "Config.hpp"

class Server {
	private:
		SocketHandler socketHandler;
		std::vector<SSocket> sources;
		std::vector<Config> configs;
	protected:
	public:
		bool startUp(std::string const &pathConfig);
		bool serverLoop();
		bool down();
};
