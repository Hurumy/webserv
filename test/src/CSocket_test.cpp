/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:35:08 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 13:14:17 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "CSocket.hpp"

TEST(CSocketTest, constructorTest) {
	CSocket csocket(3);

	ASSERT_EQ(csocket.getSockfd(), 3);
	ASSERT_EQ(csocket.getRevents(), 0);
}

TEST(CSocketTest, setReventsTest) {
	CSocket csocket(3);

	ASSERT_EQ(csocket.getSockfd(), 3);
	ASSERT_EQ(csocket.getRevents(), 0);
	csocket.setRevents(2);
	ASSERT_EQ(csocket.getRevents(), 2);
}
