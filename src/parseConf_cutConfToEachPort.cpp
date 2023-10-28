/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf_cutConfToEachPort.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:11:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/27 14:53:30 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

//コメントを丸ごと切り出す
static std::string cutOffStr(std::string &origin, std::string start,
							 std::string end) {
	size_t startpos;
	size_t endpos;
	size_t offset = 0;
	std::string prev;
	std::string next;
	std::string result = origin;

	while (1) {
		startpos = origin.find(start, offset);
		if (startpos == std::string::npos) break;
		endpos = origin.find(end, startpos);
		if (endpos == std::string::npos) break;
		prev = origin.substr(0, startpos);
		// std::cout << "prev: " << prev << std::endl;
		next = origin.substr(endpos, origin.size());
		// std::cout << "next: " << next << std::endl;
		result = prev;
		result += next;
		offset = startpos + 1;
		origin = result;
	}
	return (result);
}

//第一引数の文字列から、第二引数の文字列を検索し、その全てを第三引数の文字列に置き換える
std::string replaceStr(std::string &origin, std::string search,
					   std::string replace) {
	size_t pos = 0;
	size_t offset = 0;
	const size_t searchlen = search.length();
	const size_t replen = replace.length();

	while (pos != std::string::npos || search.empty() == false) {
		pos = origin.find(search, offset);
		if (pos == std::string::npos) break;
		origin.replace(pos, searchlen, replace);
		offset = pos + replen;
	}
	return (origin);
}

//一つの命令ごとに切り分ける
std::vector<std::string> cutConfToEachPort(std::string raw) {
	std::vector<std::string> lines;

	// delete&ignore comments
	cutOffStr(raw, "#", "\n");

	// std::cout << raw << std::endl;

	// interpret spaces as " "
	replaceStr(raw, "\n", " ");
	replaceStr(raw, "\r", " ");
	replaceStr(raw, "\t", " ");
	replaceStr(raw, "\v", " ");
	replaceStr(raw, "\f", " ");
	replaceStr(raw, "{", ";{;");
	replaceStr(raw, "}", ";};");

	// cut string by ";"
	lines = lineSpliter(raw, ";");

	return (lines);
}
