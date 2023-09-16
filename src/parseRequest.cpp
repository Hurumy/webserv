/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:16 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/16 12:28:51 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "webserv.hpp"

//単純なSplit Delimは完全一致
std::vector<std::string> lineSpliter(std::string origin, std::string delim) {
	std::vector<std::string> list;
	long pos;
	int delimlen = delim.length();
	int offset;

	if (delimlen == 0)
		list.push_back(origin);
	else {
		offset = 0;
		pos = 0;
		while (1) {
			while (origin.find(delim, offset) + delimlen ==
				   origin.find(delim, offset + delimlen))
				offset += delimlen;
			pos = origin.find(delim, offset);
			if ((unsigned long)pos == std::string::npos) {
				list.push_back(origin.substr(offset));
				break;
			}
			list.push_back(origin.substr(offset, pos - offset));
			offset = pos + delimlen;
		}
	}
	return (list);
}

// 1行目をパースしてRequestに中身をセットする
static bool parseFirstLine(Request &req, std::string Firstline) {
	std::vector<std::string> elems;
	std::vector<std::string> tmp;

	elems = lineSpliter(Firstline, " ");
	if (elems.size() >= 3) {
		req.setMethod(elems.at(0));
		req.setUrl(elems.at(1));
		tmp = lineSpliter(elems.at(2), "\r");
		req.setVersion(tmp.at(0));
	} else
		return (false);
	return (true);
}

//ヘッダーのうち1行をもらい、RequestにHeaderとして詰める
static bool addParsedLine(Request &req, std::string line) {
	std::vector<std::string> tmp;
	bool status;

	tmp = lineSpliter(line, ": ");
	if (tmp.size() != 2) return (false);
	status = req.addHeader(tmp.at(0), tmp.at(1));
	if (status == false) return (false);
	return (true);
}

static bool	getRequestBody(Request &req, std::vector<std::string> lines)
{
	std::stringstream	ss;
	std::string			str;
	unsigned long long	bytes = 0;


	//Bodyが存在するかチェックする
	//req.getHeader("Content-Type")
	
	//Bodyのバイト数を見る
	Result<std::string, bool> result = getHeader("Content-Length");
	if (result.isError() == true)
		return (false);
	
	//バイト数分読み込んで詰める
	str = result.getOk();
	ss << str;
	ss >> bytes;


}

//リクエストをパースして参照に詰める
bool parseRequest(Request &req, std::string rawData) {
	std::vector<std::string> lines;
	bool status;

	lines = lineSpliter(rawData, "\n");
	status = parseFirstLine(req, lines.at(0));
	if (status == false) return (false);
	for (unsigned long i = 1; i < lines.size(); i++) {
		if (lines.at(i) == "")
			break ;
		status = addParsedLine(req, lines.at(i));
	}

	return (true);
}

/*
int main()
{
	Request test;
	std::vector<std::string> lines;

	test.addHeader("test", "value");
	std::cout << test.getHeader("test") << std::endl;
	lines = lineSpliter("test1, test2, test3, what is this?", ", ");
	for (int i = 0; i < lines.size(); i ++)
		std::cout << lines.at(i) << std::endl;
	std::cout << "====" << std::endl;

	parseRequest(test, "GET / HTTP/1.1\t\nHost: xxx\nUser-Agent:
FireFox\nAccept: ???\n\n");

	std::cout << test.getMethod() << std::endl;
	std::cout << test.getUrl() << std::endl;
	std::cout << test.getVersion() << std::endl;

	std::cout << test.getHeader("Host") << std::endl;
	std::cout << test.getHeader("User-Agent") << std::endl;
	std::cout << test.getHeader("Accept") << std::endl;

	return (0);
}
*/
