/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_index.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:20:57 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/03 12:38:40 by komatsud         ###   ########.fr       */
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

	if (conf.getIndex().empty() == false) errorInInit("Too many index declare(´-ω-`)");

	// std::cout << lines.size() << std::endl;
	for (size_t i = 1; i < lines.size(); i++) {
		// std::cout << lines.at(i) << std::endl;
		conf.addIndex(lines.at(i));
	}

	return (0);
}
