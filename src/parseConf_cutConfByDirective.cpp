/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf_cutConfByDirective.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:04:53 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 12:36:07 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

// Configファイルを開いて一本のStringにする
static Result<std::string, bool> openAndReadConf(std::string filepath) {
	int fd;
	ssize_t status;
	char buf[FILE_READ_SIZE + 1];
	std::string rawdata;

	fd = open(filepath.c_str(), O_RDONLY);
	if (fd < 0) {
		errorInInit("Failed to open the config file｡°(´ฅωฅ`)°｡");
		return Error<bool>(false);
	}

	do {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0) {
			buf[status] = '\0';
			rawdata.append(buf);
		}
	} while (status > 0);

	if (status == -1) {
		errorInInit("Failed to read the config file｡°(´ฅωฅ`)°｡");
		return Error<bool>(false);
	}

	// std::cout << rawdata << std::endl;

	return Ok<std::string>(rawdata);
}

//{ == }であることを確かめる
static bool countParentheses(std::string rawdata, std::string start,
							 std::string end) {
	size_t num_of_start = 0;
	size_t num_of_end = 0;
	size_t offset = 0;
	long pos = 0;

	while (1) {
		pos = rawdata.find(start, offset);
		if ((unsigned long)pos == std::string::npos) break;
		num_of_start++;
		offset = pos + start.length();
	}

	offset = 0;
	pos = 0;

	while (1) {
		pos = rawdata.find(end, offset);
		if ((unsigned long)pos == std::string::npos) break;
		num_of_end++;
		offset = pos + end.length();
	}

	if (num_of_start == num_of_end)
		return (true);
	else {
		errorInInit(
			"Inconsistent number of parentheses in the Config file（>Д<）");
		return (false);
	}
}

//第1層をパース
static std::vector<std::string> cutPorts(std::string rawdata, std::string start,
										 std::string end) {
	std::vector<std::string> list;
	long pos;
	const int startlen = start.length();
	const int endlen = end.length();
	int offset;
	int depth;
	int foundstart;
	int foundend;

	if (startlen == 0 || endlen == 0)
		list.push_back(rawdata);
	else {
		offset = 0;
		pos = 0;
		depth = 0;

		while (1) {
			foundstart = rawdata.find(start, offset);
			foundend = rawdata.find(end, offset);

			if (foundstart <= foundend) {
				if ((unsigned long)foundstart == std::string::npos) {
					list.push_back(rawdata.substr(pos, foundstart - pos));
					break;
				}
				depth++;
				offset = foundstart + startlen;
			} else if (foundend < foundstart) {
				if ((unsigned long)foundend == std::string::npos) {
					list.push_back(
						rawdata.substr(pos, foundend + endlen - pos));
					break;
				}
				depth--;
				offset = foundend + endlen;
				if (depth == 0) {
					list.push_back(
						rawdata.substr(pos, foundend + endlen - pos));
					pos = foundend + endlen;
				}
			}
		}
	}
	return (list);
}

//読み込んだConfファイルを、ディレクティブごとに分けてVectorで返す
Result<std::vector<std::string>, bool> cutConfByDirective(
	std::string filepath) {
	bool status;
	std::string rawdata;
	std::vector<std::string> firstlayer;

	//まずファイルの中身を読み出す
	Result<std::string, bool> res = openAndReadConf(filepath);

	if (res.isOK() == false) return Error<bool>(false);

	rawdata = res.getOk();

	//カッコの数を数えて{==}でなければ弾く
	status = countParentheses(rawdata, "{", "}");
	if (status == false) return Error<bool>(false);

	//ここで#でコメントアウトされた行を削除する
	std::string trimmedstring = rawdata;
	std::string p;
	std::string s;
	size_t offset = 0;
	size_t start;
	size_t end;
	while (1) {
		//#を探す
		start = trimmedstring.find('#', offset);
		if (start == std::string::npos) break;

		//#の直後にある改行を探す
		end = trimmedstring.find('\n', start + 1);
		if (end == std::string::npos) {
			//#の後に改行かなかった時は、EOFで挟まれているときなので、ここでBreakしない
			end = trimmedstring.size();
		}

		//＃までと＃からに切り分け、それをくっつけてループ
		p = trimmedstring.substr(0, start);
		s = trimmedstring.substr(end, trimmedstring.size());
		trimmedstring = p + s;

		offset = start;

		// std::cout << trimmedstring << std::endl;
	}

	//切り出した後のStringをRawdataとして格納し直す
	rawdata = trimmedstring;

	//第一層の頭とカッコを数え、その中身をVectorに切り出す
	firstlayer = cutPorts(rawdata, "{", "}");

	// for(size_t i = 0; i < firstlayer.size(); i ++)
	// {
	// 	std::cout << YELLOW "vector " << i << ": " << firstlayer.at(i) << RESET
	// << std::endl; 	std::cout << RED "===ONE DIRECTIVE END===" << RESET <<
	// std::endl;
	// }

	return Ok<std::vector<std::string> >(firstlayer);
}
