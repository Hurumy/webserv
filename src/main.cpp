/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:15:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/07 12:23:36 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ConfParser.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "RequestHandler.hpp"
#include "Result.hpp"
#include "Server.hpp"
#include "SocketHandler.hpp"
#include "ft.hpp"

int main(const int argc, const char **argv) {
	try {
		if (argc != 2) {
			ft::errorInInit("Usage: ./webserv <conf_file_path>");
			return 1;
		}

		Server server;

		if (server.startUp(std::string(argv[1])) == false) return 1;
		if (server.serverLoop() == false) {
			return 1;
		}
		if (server.down() == false) {
			return 1;
		}
	} catch (...) {
		ft::putSystemError("fatal");
		return 1;
	}
	return 0;
}
