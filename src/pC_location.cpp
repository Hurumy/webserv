/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_location.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:13:21 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/16 10:23:17 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

// location / のように、Locationが設定されているURIを読み込んでいる
int l_readLocation(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "location")
		ft::errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		ft::errorInInit(
			"There are no settings in some Location directive(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.at(1).empty() == false) {
		loc.setUri(lines.at(1));
	}

	return (0);
}
