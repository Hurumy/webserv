/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_index.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:20:57 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/12 11:25:58 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readIndex(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "index")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() < 2) errorInInit("There are no settings(ﾉｼ｀･ω･)ﾉｼ");

	if (conf.getIndex().empty() == false)
		errorInInit("Too many index is declare(´-ω-`)");

	// std::cout << lines.size() << std::endl;
	for (size_t i = 1; i < lines.size(); i++) {
		// std::cout << lines.at(i) << std::endl;
		conf.addIndex(lines.at(i));
	}

	return (0);
}

int l_readIndex(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "index")
		errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() < 2) errorInInit("There are no settings(ﾉｼ｀･ω･)ﾉｼ");

	if (loc.getIndex().empty() == false)
		errorInInit("Too many index is declare(´-ω-`)");

	// std::cout << lines.size() << std::endl;
	for (size_t i = 1; i < lines.size(); i++) {
		// std::cout << lines.at(i) << std::endl;
		loc.addIndex(lines.at(i));
	}

	return (0);
}
