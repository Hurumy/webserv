/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_listen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:55:31 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 15:52:02 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static int setPortNumber(std::string line, Address &add) {
	int tmp;
	unsigned long long port;
	std::stringstream ss;

	//ポート番号があるはずの文字列の中身が全て数字であることを確認
	tmp = isNumber(line);
	if (tmp == false)
		errorInInit("There are characters not number in Port Numbers ( ◜◡‾)\n");

	//文字列を数字に変換する
	ss << line;
	ss >> port;

	//ポート番号が有効であることを確認する
	if (!(1 <= port && port <= 65535))
		errorInInit(
			"Invalid Port Number detected in http:server:listen directive! ⊂(  "
			"っ☉ω☉)っ\n");

	add.setPort(port);
	return (0);
}

//このOnelineの中身はポート番号のみ
static int checkProtocol(std::string &oneline, Address &add) {
	size_t start;
	size_t end;
	size_t tmp;
	size_t tmp2;
	std::vector<std::string> lines;

	start = oneline.find("[", 0);
	end = oneline.find("]", 0);

	//[]がそれぞれ一つずつしかないことを確認する
	tmp = oneline.find("[", start + 1);
	tmp2 = oneline.find("]", end + 1);
	if (tmp != std::string::npos || tmp2 != std::string::npos)
		errorInInit("too many [] signs detected!! (ﾉω<､)\n");

	if (start == std::string::npos && end == std::string::npos) {
		//プロトコルをIPv4にセットする
		add.setIpVers(0);

		// v4アドレス部分とポート番号部分に分ける
		lines = lineSpliter(oneline, ":");

		if (lines.size() != 2)
			errorInInit(
				"Invalid number of elements detected in http:server:listen "
				"directive Σ(・ω・ノ)ノ\n");

		//ポートをセットする
		setPortNumber(lines.at(1), add);

		// IPv4アドレスをセットする
		add.setIpAddress(lines.at(0));
	} else if (start != std::string::npos && end != std::string::npos &&
			   start <= end) {
		//プロトコルをIPv6にセットする
		add.setIpVers(1);

		//"["を消し、"]"で文字列を切り分ける(v6アドレスとポートに分かれる)
		replaceStr(oneline, "[", "");
		lines = lineSpliter(oneline, "]");

		if (lines.size() != 2)
			errorInInit(
				"Invalid number of elements detected in http:server:listen "
				"directive Σ(・ω・ノ)ノ\n");

		//ポートをセットするために、ポート番号の前にある:を消し、関数に送る
		replaceStr(lines.at(1), ":", "");
		setPortNumber(lines.at(1), add);

		// IPv6アドレスをセットする
		add.setIpAddress(lines.at(0));
	} else
		errorInInit(
			"Invalid string detected in http:server:listen directive "
			"(ﾉ´・ω・)ﾉ⌒✹\n");

	return (0);
}

int readListen(Config &conf, std::string oneline) {
	std::vector<std::string> lines;
	Address add;
	unsigned long long status;

	// IP設定の有無を確認
	status = oneline.find(":", 0);

	//スペースで区切る。と、"listen" "(IP):8080"のように切れるはず
	lines = lineSpliter(oneline, " ");

	//空の文字列を削除して寄せる
	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	// default_serverが設定されていた時
	if (lines.size() == 3 && lines.at(2) == "default_server")
		errorInInit(
			"I'm literally so sorry but you cant set default_server in this "
			"webserv (ヾﾉ･ω･`)\n");

	//スペースで区切った後の要素数が2つではないとき
	if (lines.size() != 2 || lines.at(0) != "listen")
		errorInInit("invalid number of elements (๑╹ω╹๑ )\n");

	//:があるかないか(IPの設定があるかないかを確認する)
	if (status != std::string::npos)
		checkProtocol(lines.at(1), add);
	else  //:がないとき
		setPortNumber(lines.at(1), add);

	conf.addAddresses(add);
	// std::cout << BLUE "port: " << conf.getAddresses().at(0).getPort() <<
	// RESET << std::endl;

	return (0);
}
