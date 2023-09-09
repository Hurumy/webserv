/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler_test.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:14:49 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 16:49:43 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "SocketHandler.hpp"

TEST(SocketHandlerTest, constructorTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000);
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	ASSERT_EQ(ssockets.at(0).getPort(), 8080);
	ASSERT_EQ(ssockets.at(1).getPort(), 8000);
	ASSERT_EQ(ssockets.at(2).getPort(), 4040);
	socketHandler.addCSocket(CSocket(0));
	std::vector<CSocket> const &csockets = socketHandler.getCSockets();
	ASSERT_EQ(csockets.at(0).getRevents(), 0);
}

TEST(SocketHandlerTest, pollfdsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.createPollfds();
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	std::vector<struct pollfd> const &pollfds = socketHandler.getPollfds();
	ASSERT_EQ(pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(pollfds.at(0).events, POLLIN | POLLOUT | POLLHUP);
}

TEST(SocketHandlerTest, setReventsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.createPollfds();
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	std::vector<struct pollfd> const &pollfds = socketHandler.getPollfds();
	ASSERT_EQ(pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(pollfds.at(0).events, POLLIN | POLLOUT | POLLHUP);
	ASSERT_TRUE(socketHandler.setRevents());
	std::vector<struct pollfd> const &resutl_pollfds = socketHandler.getPollfds();
	ASSERT_EQ(resutl_pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(resutl_pollfds.at(0).revents, ssockets.at(0).getRevents());
	ASSERT_EQ(resutl_pollfds.at(1).revents, ssockets.at(1).getRevents());
}

TEST(SocketHandlerTest, recieveCSocketsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 1));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	pid_t pid = fork();
	if (pid == -1) {
		std::exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		sleep(1);
		system("curl localhost:8080");
		std::exit(EXIT_SUCCESS);
	}
	while (true) {
		socketHandler.recieveCSockets();
		if (socketHandler.getCSockets().size() != 0) {
			break ;
		}
		socketHandler.clearPollfds();
		socketHandler.createPollfds();
		socketHandler.setRevents();
	}
	socketHandler.closeAllSSockets();
}
