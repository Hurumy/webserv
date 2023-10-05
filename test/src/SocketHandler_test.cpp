/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler_test.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:14:49 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/03 13:29:10 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <gtest/gtest.h>

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

TEST(SocketHandlerTest, constructorTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000, 100);
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
	SocketHandler socketHandler(sources, 1000, 100);
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
	SocketHandler socketHandler(sources, 1000, 100);
	socketHandler.createPollfds();
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	std::vector<struct pollfd> const &pollfds = socketHandler.getPollfds();
	ASSERT_EQ(pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(pollfds.at(0).events, POLLIN | POLLOUT | POLLHUP);
	ASSERT_TRUE(socketHandler.setRevents());
	std::vector<struct pollfd> const &resutl_pollfds =
		socketHandler.getPollfds();
	ASSERT_EQ(resutl_pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(resutl_pollfds.at(0).revents, ssockets.at(0).getRevents());
	ASSERT_EQ(resutl_pollfds.at(1).revents, ssockets.at(1).getRevents());
}
