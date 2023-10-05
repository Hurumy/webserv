/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf_putEachLine.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:25:22 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/05 10:33:15 by komatsud         ###   ########.fr       */
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

	if (pos == std::string::npos)
		return (-1);
	else
		return (0);
}

// Confファイルの1行1行をみてなんの設定なのか解釈する
static int checkSettings(Config &conf, std::string oneline) {
	std::vector<std::string> lines;
	//std::cout << oneline << std::endl;
	int status = 0;

	lines = lineSpliter(oneline, " ");
	for (size_t i = 0; i < lines.size(); i++)
	{
		//  std::cout << YELLOW ": " << lines.at(i) << RESET << std::endl;
		//  std::cout << i << ", " << lines.size() << std::endl;

		if (lines.at(i).empty() == false) {
			if (lines.at(i) == "server") {
				break ;
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

// Location	checkLocationSettings(std::vector<std::string>	const &line)
// {
// 	Location	res;
// 	int			status;

// 	for (size_t i = 0; i < line.size(); i++)
// 	{
// 		std::cout << YELLOW ": " << line.at(i) << RESET << std::endl;

// 		if (line.at(i).empty() == false)
// 		{
// 			if (line.at(i) == "server") {
// 				break ;
// 			} else if (line.at(i) == "listen") {
// 				status = readListen(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "server_name") {
// 				status = readServerName(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "root") {
// 				status = readRoot(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "error_page") {
// 				status = readErrorPage(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "return") {
// 				status = readReturn(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "rewrite") {
// 				status = readRewrite(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "autoindex") {
// 				status = readAutoindex(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "client_max_body_size") {
// 				status = readMaxBodySize(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "index") {
// 				status = readIndex(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "upload_path") {
// 				status = readUploadPath(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "allowedMethods") {
// 				status = readAllowedMethods(conf, oneline);
// 				break;
// 			} else if (line.at(i) == "{" || line.at(i) == "}") {
// 				break;
// 			} else if (line.at(i).empty() == false) {
// 				std::cout << RED << line.at(i) << RESET << std::endl;
// 				errorInInit("Unknown directive was detected.(｀・ω・´)");
// 			}
// 			if (status == -1) break;
// 		}
// 	}
// 	return res;
// }

Result<std::vector<std::string>, bool>	cutOutLocation(const std::vector<std::string> &origin)
{
	size_t	hed_pos;			 // "location /"
	size_t	st_pos; 			 //  "{"
	size_t	end_pos;			 //  "}"
	std::vector<std::string>	line;
	std::vector<std::string>	tmp;

	//originをLineにコピー
	for (size_t a = 0; a < origin.size(); a ++)
	{
		line.push_back(origin.at(a));
		//std::cout << line.at(a) << std::endl;
	}


	//"location" というディレクティブが含まれるかどうかチェックする
	for (size_t i = 0; i < line.size(); i ++)
	{
		// "location"を探す
		hed_pos = line.at(i).find("location");
		if (hed_pos != std::string::npos && i < line.size() - 1)
		{
			// "location"の後にある{を探す
			st_pos = line.at(i + 1).find("{");
			if (st_pos != std::string::npos)
			{
				// {の直後にある}を探す
				for (size_t j = i + 1; j < line.size(); j ++)
				{
					end_pos = line.at(j).find("}");
					if (end_pos != std::string::npos)
					{
						//i-jまでのStringを切り出し、パースしてLocationをConfigに詰める 元のLineからi-jを削除する
						for (size_t k = i; k <= j; k ++)
							std::cout << GREY << line.at(k) << RESET << std::endl;

						tmp.erase(tmp.begin(), tmp.end());
						
						//tmpにコピーしてLineに戻す
						for (size_t k = 0; k < line.size(); k ++)
						{
							if (k < i || k > j)
							{
								tmp.push_back(line.at(k));
							}
						}
						
						line.erase(line.begin(), line.end());
						//lineにtmpをコピーする
						for (size_t k = 0; k < tmp.size(); k ++)
						{
							line.push_back(tmp.at(k));
						}

						for (size_t l = 0; l < line.size(); l ++)
							std::cout << RED << line.at(l) << RESET << std::endl;
					}
				}
			}
		}
	}

	//含まれた場合は切り出し、解釈して、ConfigにLocationクラスを追加する

	//Locationディレクティブの部分を切り取った文字列を返す
	return Ok<std::vector<std::string> >(line);
}


Result<Config, bool> parsePortVecs(std::string port) {
	Config conf;
	std::vector<std::string> line;
	int status;

	//頭が”Server”であることを確認 そうでなければスキップ
	status = isServerSetting(port);
	if (status == -1) return Error<bool>(false);

	//std::cout << port << std::endl;

	//セミコロンごとにきりわける
	line = cutConfToEachPort(port);

	// for (size_t i = 0; i < line.size(); i ++)
	// 	std::cout << BLUE << line.at(i) << RESET << std::endl;
	

	//ここでLocationディレクティブを切り出していき、Locationに詰め、Configのベクターにしまう
	//後に流すstd::vector<std::string>にはLocationのディレクティブは含まれないようにする
	Result<std::vector<std::string>, bool>	res = cutOutLocation(line);
	if (res.isOK() == false)
	{
		errorInInit("parsing Location is failed(´ω`)");
	}
	else
	{
		line = res.getOk();
	}

	// for (size_t i = 0; i < line.size() - 1; i ++)
	// {
	// 	std::cout << RED << line.at(i) << RESET << std::endl;
	// 	std::cout << i << ", " << line.size() << std::endl;
	// }

	//セミコロンで切り分けられるひとかたまりを見て、Confに中身を詰める
	for (size_t i = 0; i < line.size(); i ++) {
		if (line.at(i).empty() == false)
		{
			//std::cout << i << ", " << line.size() << std::endl;
			status = checkSettings(conf, line.at(i));
			if (status == -1)
				return Error<bool>(false);
		}
	}

	//返す
	return Ok<Config>(conf);
}
