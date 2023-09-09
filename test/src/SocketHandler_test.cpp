/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler_test.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:14:49 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 13:33:04 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "SocketHandler.hpp"

TEST(SocketHandlerTest, constructorTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources);
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	ASSERT_EQ(ssockets.at(0).getPort(), 8080);
	ASSERT_EQ(ssockets.at(1).getPort(), 8000);
	ASSERT_EQ(ssockets.at(2).getPort(), 4040);
	socketHandler.addCSocket(CSocket(0));
	std::vector<CSocket> const &csockets = socketHandler.getCSockets();
	ASSERT_EQ(csockets.at(0).getRevents(), 0);
}
