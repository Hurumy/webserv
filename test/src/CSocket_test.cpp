/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:35:08 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/26 11:29:15 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CSocket.hpp"

#include <gtest/gtest.h>

TEST(CSocketTest, constructorTest) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	unsigned int expected(8080);

	ASSERT_EQ(csocket.getSockfd(), 3);
	ASSERT_EQ(csocket.getRevents(), 0);
	ASSERT_EQ(csocket.getLocalAddr(), "127.0.0.1");
	ASSERT_EQ(csocket.getLocalPort(), expected);
}

TEST(CSocketTest, setReventsTest) {
	CSocket csocket(3, 2110443574, "127.0.0.1", 8080);
	unsigned int expected(8080);

	ASSERT_EQ(csocket.getSockfd(), 3);
	ASSERT_EQ(csocket.getRevents(), 0);
	ASSERT_EQ(csocket.getLocalAddr(), "127.0.0.1");
	ASSERT_EQ(csocket.getLocalPort(), expected);
	csocket.setRevents(2);
	ASSERT_EQ(csocket.getRevents(), 2);
}
