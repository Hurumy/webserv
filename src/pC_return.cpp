/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_return.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:59:23 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/07 12:20:31 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static int threecontents(Config &conf, std::vector<std::string> lines) {
	std::stringstream ss;
	int num;

	// 1つ目 ステータスしかない
	if (isNumber(lines.at(1)) == true) {
		ss << lines.at(1);
		ss >> num;
		if (!(100 <= num && num < 600))
			ft::errorInInit(
				"Invalid Status Code detected in return directive (◞‸◟)");
		conf.setReturnStatus(num);
		conf.setIsReturn(true);
	} else
		ft::errorInInit(
			"Unknown element detected in return directive ヾ(ﾟω｡ヽ≡ﾉﾟω｡)ﾉﾞ");

	// 2つ目
	// 300番代の時はリダイレクト先のURL、それ以外のステータスの場合はBodyに設定する文章を任意に設定できる
	if ((lines.at(2).compare(0, 7, "http://") == 0 ||
		 lines.at(2).compare(0, 8, "https://") == 0) &&
		(300 <= num && num < 400)) {
		conf.setReturnUrl(lines.at(2));
	} else if (!(300 <= num && num < 400)) {
		conf.setReturnBody(lines.at(2));
	} else
		ft::errorInInit(
			"Unknown element detected in return directive ヾ(ﾟω｡ヽ≡ﾉﾟω｡)ﾉﾞ");

	return (0);
}

static int twocontents(Config &conf, std::vector<std::string> lines) {
	std::stringstream ss;
	int num;

	//要素数が1つのときはステータスかURLしか入らない
	// URLの時はステータスを302にしてよい
	if (isNumber(lines.at(1)) == true) {
		ss << lines.at(1);
		ss >> num;
		if (!(100 <= num && num < 600))
			ft::errorInInit(
				"Invalid Status Code detected in return directive (◞‸◟)");
		conf.setReturnStatus(num);
		conf.setIsReturn(true);
	} else if (lines.at(1).compare(0, 7, "http://") == 0 ||
			   lines.at(1).compare(0, 8, "https://") == 0) {
		conf.setReturnUrl(lines.at(1));
		conf.setReturnStatus(302);
		conf.setIsReturn(true);
	} else
		ft::errorInInit(
			"Unknown element detected in return directive ヾ(ﾟω｡ヽ≡ﾉﾟω｡)ﾉﾞ");

	return (0);
}

int readReturn(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "return")
		ft::errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() == 2)
		return (twocontents(conf, lines));
	else if (lines.size() == 3)
		return (threecontents(conf, lines));
	else
		ft::errorInInit("Invalid form detected in return directives ⊂('ω`⊂ 三");

	return (0);
}

static int l_threecontents(Location &loc, std::vector<std::string> lines) {
	std::stringstream ss;
	int num;

	// 1つ目 ステータスしかない
	if (isNumber(lines.at(1)) == true) {
		ss << lines.at(1);
		ss >> num;
		if (!(100 <= num && num < 600))
			ft::errorInInit(
				"Invalid Status Code detected in return directive (◞‸◟)");
		loc.setReturnStatus(num);
		loc.setIsReturn(true);
	} else
		ft::errorInInit(
			"Unknown element detected in return directive ヾ(ﾟω｡ヽ≡ﾉﾟω｡)ﾉﾞ");

	// 2つ目
	// 300番代の時はリダイレクト先のURL、それ以外のステータスの場合はBodyに設定する文章を任意に設定できる
	if ((lines.at(2).compare(0, 7, "http://") == 0 ||
		 lines.at(2).compare(0, 8, "https://") == 0) &&
		(300 <= num && num < 400)) {
		loc.setReturnUrl(lines.at(2));
	} else if (!(300 <= num && num < 400)) {
		loc.setReturnBody(lines.at(2));
	} else
		ft::errorInInit(
			"Unknown element detected in return directive ヾ(ﾟω｡ヽ≡ﾉﾟω｡)ﾉﾞ");

	return (0);
}

static int l_twocontents(Location &loc, std::vector<std::string> lines) {
	std::stringstream ss;
	int num;

	//要素数が1つのときはステータスかURLしか入らない
	// URLの時はステータスを302にしてよい
	if (isNumber(lines.at(1)) == true) {
		ss << lines.at(1);
		ss >> num;
		if (!(100 <= num && num < 600))
			ft::errorInInit(
				"Invalid Status Code detected in return directive (◞‸◟)");
		loc.setReturnStatus(num);
		loc.setIsReturn(true);
	} else if (lines.at(1).compare(0, 7, "http://") == 0 ||
			   lines.at(1).compare(0, 8, "https://") == 0) {
		loc.setReturnUrl(lines.at(1));
		loc.setReturnStatus(302);
		loc.setIsReturn(true);
	} else
		ft::errorInInit(
			"Unknown element detected in return directive ヾ(ﾟω｡ヽ≡ﾉﾟω｡)ﾉﾞ");

	return (0);
}

int l_readReturn(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "return")
		ft::errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() == 2)
		return (l_twocontents(loc, lines));
	else if (lines.size() == 3)
		return (l_threecontents(loc, lines));
	else
		ft::errorInInit("Invalid form detected in return directives ⊂('ω`⊂ 三");

	return (0);
}
