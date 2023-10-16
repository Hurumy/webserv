/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 14:28:47 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 14:42:17 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Address.hpp"

#include <gtest/gtest.h>

#include "webserv.hpp"

TEST(AddressTest, ipAddressTest) {
	Address add;
	std::string expected("111.108.92.125:8660");

	add.setIpAddress(expected);
	ASSERT_EQ(add.getIpAddress(), expected);
}

TEST(AddressTest, portTest) {
	Address add;
	int expected(8080);

	add.setPort(expected);
	ASSERT_EQ(add.getPort(), expected);
}

TEST(AddressTest, ipversTest) {
	Address add;
	int expected(1);

	add.setIpVers(expected);
	ASSERT_EQ(add.getIpVers(), expected);
}
