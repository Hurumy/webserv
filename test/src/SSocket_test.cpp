/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:07:23 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/26 11:30:52 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SSocket.hpp"

#include <gtest/gtest.h>

TEST(SSocketTest, constructorTest) {
	SSocket ssocket("0.0.0.0", 8080, IPV4, 100);
	unsigned int expected(8080);

	ASSERT_EQ(ssocket.getPort(), expected);
	ASSERT_EQ(ssocket.getIpVer(), IPV4);
	ASSERT_EQ(ssocket.getBacklog(), 100);
}

TEST(SSocketTest, SSocketInitTest) {
	SSocket ssocket("0.0.0.0", 8080, IPV4, 100);
	unsigned int expected(8080);

	ASSERT_TRUE(ssocket.init());
	std::clog << "sockfd: " << ssocket.getSockfd() << std::endl;
	ASSERT_EQ(ssocket.getPort(), expected);
	ASSERT_EQ(ssocket.getIpVer(), IPV4);
	ASSERT_EQ(ssocket.getBacklog(), 100);
	ssocket.closeSockfd();
}
