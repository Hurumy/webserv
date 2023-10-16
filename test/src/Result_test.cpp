/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Result_test.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 11:40:14 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/15 17:53:30 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Result.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "Error.hpp"
#include "Ok.hpp"

Result<int, std::string> parse_digit(char c) {
	if (c < '0' || '9' < c) {
		return Error<std::string>("invalid character");
	}
	return Ok<int>(c - '0');
}

TEST(ResultTest, OkTest) {
	Result<int, std::string> result = parse_digit('0');

	ASSERT_EQ(result.isOK(), true);
	ASSERT_EQ(result.isError(), false);
	ASSERT_EQ(result.getOk(), 0);
}

TEST(ResultTest, ErrorTest) {
	Result<int, std::string> result = parse_digit('a');
	std::string expected("invalid character");

	ASSERT_EQ(result.isOK(), false);
	ASSERT_EQ(result.isError(), true);
	ASSERT_STREQ(result.getError().c_str(), expected.c_str());
}

TEST(ResultTest, CopyTest) {
	Result<int, std::string> result = parse_digit('a');
	std::string expected("invalid charactertest");
	std::string str = result.getError();
	str += "test";

	ASSERT_STREQ(str.c_str(), expected.c_str());
}
