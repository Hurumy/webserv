/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeDirListHTML_test.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:26:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/17 14:35:09 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MakeDirlistHTML.hpp"

#include <gtest/gtest.h>

TEST(MakeDirListHTMLTest, writeOutTest)
{
	MakeDirlistHTML tmp("./www/content/");

	std::cout << tmp.returnHTML().getOk() << std::endl;
}
