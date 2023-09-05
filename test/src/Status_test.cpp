/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:15:45 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/05 13:04:11 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include <string>
#include "Status.hpp"

TEST(StatusTest, statusTest) {
	std::string expected("OK");

	ASSERT_EQ(Status::getStatusMessage(200).isOK(), true);
	ASSERT_EQ(Status::getStatusMessage(200).isError(), false);
	ASSERT_STREQ(Status::getStatusMessage(200).getOk().c_str(), expected.c_str());
}

TEST(StatusTest, statusErrorTest) {
	ASSERT_EQ(Status::getStatusMessage(1000).isOK(), false);
	ASSERT_EQ(Status::getStatusMessage(1000).isError(), true);
	ASSERT_EQ(Status::getStatusMessage(1000).getError(), false);
}
