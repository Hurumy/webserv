/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf_putEachLine.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:25:22 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 17:33:07 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

//そのディレクティブにServerと書かれていることを確認する
static int		isServerSetting(std::string raw)
{
	unsigned long	pos;
	std::vector<std::string> lines = lineSpliter(raw, "{");
	std::string		line = lines.at(0);

	pos = line.find("server", 0);

	if (pos == std::string::npos)
		return (-1);
	else
		return (0);
}

//Confファイルの1行1行をみてなんの設定なのか解釈する
static int	checkSettings(Config &conf, std::string	oneline)
{
	std::vector<std::string>	lines;
	int							status = 0;

	lines = lineSpliter(oneline, " ");
	for (size_t i = 0; i < lines.size(); i ++)
	{
		//std::cout << YELLOW ": " << lines.at(i) << RESET << std::endl;		
		
		if(lines.at(i).empty() == false)
		{
			if (lines.at(i) == "listen")
			{
				status = readListen(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "server_name")
			{
				status = readServerName(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "root")
			{
				status = readRoot(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "error_page")
			{
				status = readErrorPage(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "return")
			{
				status = readReturn(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "rewrite")
			{
				status = readRewrite(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "autoindex")
			{
				status = readAutoindex(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "client_max_body_size")
			{
				status = readMaxBodySize(conf, oneline);
				break ;
			}
			else if (lines.at(i) == "index")
			{
				status = readIndex(conf, oneline);
				break ;
			}
			if (status == -1)
				break ;
		}
	}
	return 0;
}

Result<Config, bool>	parsePortVecs(std::string port)
{
	Config								conf;
	std::vector<std::string>			line;
	int									status;

	//頭が”Server”であることを確認 そうでなければスキップ
	status = isServerSetting(port);
	if (status == -1)
		return Error<bool>(false);
	
	//セミコロンごとにきりわける
	line = cutConfToEachPort(port);
	
	//セミコロンで切り分けられるひとかたまりを見て、Confに中身を詰める
	for(size_t i = 0; i < line.size(); i ++)
	{
		if (line.at(i).empty() == false)
		{
			status = checkSettings(conf, line.at(i));
			if (status == -1)
				return Error<bool>(false);
		}
	}
	
	//返す
	return Ok<Config>(conf);
}


