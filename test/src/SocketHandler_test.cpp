/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler_test.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:14:49 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/30 16:10:14 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <gtest/gtest.h>

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

TEST(SocketHandlerTest, constructorTest) {
	std::vector<SSocket> sources;
	unsigned int expected00(8080);
	unsigned int expected01(8000);
	unsigned int expected02(4040);

	sources.push_back(SSocket("0.0.0.0", 8080, IPV4, 100));
	sources.push_back(SSocket("0.0.0.0", 8000, IPV4, 100));
	sources.push_back(SSocket("0.0.0.0", 4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000, 100);
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	ASSERT_EQ(ssockets.at(0).getPort(), expected00);
	ASSERT_EQ(ssockets.at(1).getPort(), expected01);
	ASSERT_EQ(ssockets.at(2).getPort(), expected02);
	socketHandler.addCSocket(CSocket(3, 2110443574, "127.0.0.1", 8080));
	std::vector<CSocket> const &csockets = socketHandler.getCSockets();
	ASSERT_EQ(csockets.at(0).getRevents(), 0);
}

TEST(SocketHandlerTest, pollfdsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket("0.0.0.0", 8080, IPV4, 100));
	sources.push_back(SSocket("0.0.0.0", 8000, IPV4, 100));
	sources.push_back(SSocket("0.0.0.0", 4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000, 100);
	socketHandler.createPollfds();
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	std::vector<struct pollfd> const &pollfds = socketHandler.getPollfds();
	ASSERT_EQ(pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(pollfds.at(0).events, POLLIN | POLLOUT | POLLHUP);
}

TEST(SocketHandlerTest, setReventsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket("0.0.0.0", 8080, IPV4, 100));
	sources.push_back(SSocket("0.0.0.0", 8000, IPV4, 100));
	sources.push_back(SSocket("0.0.0.0", 4040, IPV4, 100));
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
