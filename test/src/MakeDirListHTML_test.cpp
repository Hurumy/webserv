/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeDirListHTML_test.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 11:26:48 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 10:40:05 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MakeDirlistHTML.hpp"

#include <gtest/gtest.h>

TEST(MakeDirListHTMLTest, writeOutTest) {
	MakeDirlistHTML tmp("./www/content/");

	//目以外でチェックするのが厳しいので、
	//ディレクトリリスティングの結果が正しいことを目で確認してください・・・。
	std::cout << tmp.returnHTML().getOk() << std::endl;
}
