/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_cgi_path.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:48:54 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 13:59:01 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

//.cgi, .pyとか
int readCGIExtension(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "cgi_extension")
		errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() < 2)
		errorInInit("Too few argments in cgi_extension directives ι(´Д｀υ)");

	for (size_t i = 1; i < lines.size(); i++) {
		conf.addCgiExtension(lines.at(i));
		// std::cout << lines.at(i) << std::endl;
	}

	return (0);
}

int l_readCGIExtension(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "cgi_extension")
		errorInInit("Unknown directive detected! (ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() < 2)
		errorInInit("Too few argments in cgi_extension directives ι(´Д｀υ)");

	for (size_t i = 1; i < lines.size(); i++) {
		loc.addCgiExtension(lines.at(i));
		// std::cout << lines.at(i) << std::endl;
	}

	return (0);
}
