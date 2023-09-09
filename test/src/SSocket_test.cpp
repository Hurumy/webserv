/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SSocket_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:07:23 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/08 17:35:21 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "SSocket.hpp"

TEST(SSocketTest, SSockConstructorTest) {
	SSocket ssocket(8080, IPV4, 100);

	ASSERT_EQ(ssocket.getPort(), 8080);
	ASSERT_EQ(ssocket.getIpVer(), IPV4);
	ASSERT_EQ(ssocket.getBacklog(), 100);
}

TEST(SSocketTest, SSocketInitTest) {
	SSocket ssocket(8080, IPV4, 100);

	ASSERT_TRUE(ssocket.init());
	std::clog << "sockfd: " << ssocket.getSockfd() << std::endl;
	ASSERT_EQ(ssocket.getPort(), 8080);
	ASSERT_EQ(ssocket.getIpVer(), IPV4);
	ASSERT_EQ(ssocket.getBacklog(), 100);
}
