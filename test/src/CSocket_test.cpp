/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:35:08 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/17 20:10:00 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CSocket.hpp"

#include <gtest/gtest.h>

TEST(CSocketTest, constructorTest) {
	CSocket csocket(3, 2110443574);

	ASSERT_EQ(csocket.getSockfd(), 3);
	ASSERT_EQ(csocket.getRevents(), 0);
}

TEST(CSocketTest, setReventsTest) {
	CSocket csocket(3, 2110443574);

	ASSERT_EQ(csocket.getSockfd(), 3);
	ASSERT_EQ(csocket.getRevents(), 0);
	csocket.setRevents(2);
	ASSERT_EQ(csocket.getRevents(), 2);
}
