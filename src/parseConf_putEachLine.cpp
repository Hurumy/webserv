/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf_putEachLine.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:25:22 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/28 11:03:57 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

//そのディレクティブにServerと書かれていることを確認する
static int isServerSetting(std::string raw) {
	unsigned long pos;
	std::vector<std::string> lines = lineSpliter(raw, "{");
	std::string line = lines.at(0);

	// for (size_t i = 0; i < lines.size(); i ++)
	// 	std::cout << BLUE << lines.at(i) << RESET <<  std::endl;

	pos = line.find("server", 0);

	//一番上の階層に存在したディレクティブがServerでなかった場合はエラーを返す
	if (pos == std::string::npos)
		errorInInit("Unknown directive (OvO)");
	else
		return (0);
	return (0);
}

// Confファイルの1行1行をみてなんの設定なのか解釈する
static int checkSettings(Config &conf, std::string oneline) {
	std::vector<std::string> lines;
	// std::cout << oneline << std::endl;
	int status = 0;

	lines = lineSpliter(oneline, " ");
	for (size_t i = 0; i < lines.size(); i++) {
		//  std::cout << YELLOW ": " << lines.at(i) << RESET << std::endl;
		//  std::cout << i << ", " << lines.size() << std::endl;

		if (lines.at(i).empty() == false) {
			if (lines.at(i) == "server") {
				break;
			} else if (lines.at(i) == "listen") {
				status = readListen(conf, oneline);
				break;
			} else if (lines.at(i) == "server_name") {
				status = readServerName(conf, oneline);
				break;
			} else if (lines.at(i) == "root") {
				status = readRoot(conf, oneline);
				break;
			} else if (lines.at(i) == "error_page") {
				status = readErrorPage(conf, oneline);
				break;
			} else if (lines.at(i) == "return") {
				status = readReturn(conf, oneline);
				break;
			} else if (lines.at(i) == "rewrite") {
				status = readRewrite(conf, oneline);
				break;
			} else if (lines.at(i) == "autoindex") {
				status = readAutoindex(conf, oneline);
				break;
			} else if (lines.at(i) == "client_max_body_size") {
				status = readMaxBodySize(conf, oneline);
				break;
			} else if (lines.at(i) == "index") {
				status = readIndex(conf, oneline);
				break;
			} else if (lines.at(i) == "upload_path") {
				status = readUploadPath(conf, oneline);
				break;
			} else if (lines.at(i) == "allowedMethods") {
				status = readAllowedMethods(conf, oneline);
				break;
			} else if (lines.at(i) == "cgi_extension") {
				status = readCGIExtension(conf, oneline);
				break;
			} else if (lines.at(i) == "{" || lines.at(i) == "}") {
				break;
			} else if (lines.at(i).empty() == false) {
				std::cout << RED << lines.at(i) << RESET << std::endl;
				errorInInit("Unknown directive was detected.(｀・ω・´)");
			}
			if (status == -1) break;
		}
	}

	return 0;
}

Result<Config, bool> parsePortVecs(std::string port) {
	Config conf;
	std::vector<std::string> line;
	int status;

	//頭が”Server”であることを確認 そうでなければスキップ
	status = isServerSetting(port);
	if (status == -1) return Error<bool>(false);

	// std::cout << port << std::endl;

	//セミコロンごとにきりわける
	line = cutConfToEachPort(port);

	// for (size_t i = 0; i < line.size(); i ++)
	// 	std::cout << BLUE << line.at(i) << RESET << std::endl;

	//ここでLocationディレクティブを切り出していき、Locationに詰め、Configのベクターにしまう
	//後に流すstd::vector<std::string>にはLocationのディレクティブは含まれないようにする
	Result<std::vector<std::string>, bool> res = cutOutLocation(line, conf);
	if (res.isOK() == false) {
		errorInInit("parsing Location is failed(´ω`)");
	} else {
		line = res.getOk();
	}

	// for (size_t i = 0; i < line.size() - 1; i ++)
	// {
	// 	std::cout << RED << line.at(i) << RESET << std::endl;
	// 	std::cout << i << ", " << line.size() << std::endl;
	// }

	// Configの初期化処理(コンストラクタでやりたくないから・・・)
	conf.setDirlist(false);

	//セミコロンで切り分けられるひとかたまりを見て、Confに中身を詰める
	for (size_t i = 0; i < line.size(); i++) {
		if (line.at(i).empty() == false) {
			// std::cout << i << ", " << line.size() << std::endl;
			status = checkSettings(conf, line.at(i));
			if (status == -1) return Error<bool>(false);
		}
	}

	//重複・初期化処理ない時の処理
	if (conf.getAddresses().size() == 0) {
		thereisnoListen(conf);
		if (conf.getServerName().size() == 0)
			errorInInit("There is no server name...（＾ω＾）");
	}

	//返す
	return Ok<Config>(conf);
}