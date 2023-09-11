/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makeConf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:37:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/11 13:16:00 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

std::vector<std::string> lineSpliter(std::string origin, std::string delim);

std::string	cutOffStr(std::string &origin, std::string start, std::string end)
{
	size_t		startpos;
	size_t		endpos;
	size_t		offset = 0;
	std::string	prev;
	std::string	next;
	std::string	result = origin;

	while (1)
	{
		startpos = origin.find(start, offset);
		if (startpos == std::string::npos)
			break;
		endpos = origin.find(end, startpos);
		if (endpos == std::string::npos)
			break;
		prev = origin.substr(0, startpos);
		next = origin.substr(endpos, origin.size());
		result = prev;
		result += next;
		offset = startpos + 1;
	}
	origin = result;
	return (result);
}

std::string	replaceStr(std::string &origin, std::string search, std::string replace)
{
	size_t			pos = 0;
	size_t			offset = 0;
	const size_t	searchlen = search.length();
	const size_t	replen = replace.length();

	while (pos != std::string::npos || search.empty() == false)
	{
		pos = origin.find(search, offset);
		if (pos == std::string::npos)
			break;
		origin.replace(pos, searchlen, replace);
		offset = pos + replen;
	}
	return (origin);
}

Result<std::string, bool>	openAndReadConf(std::string filepath)
{
	int				fd;
	int				status;
	char			buf[FILE_READ_SIZE];
	std::string		rawdata;

	fd = open(filepath.c_str(), O_RDONLY);
	if (fd < 0)
		return Error<bool>(false);

	do
	{
		status = read(fd, buf, FILE_READ_SIZE);
		buf[status] = '\0';
		if (status > 0)
			rawdata += buf;			
	}
	while (status > 0);

	if (status == -1)
		return Error<bool>(false);
	
	std::cout << rawdata << std::endl;
	
	return Ok<std::string>(rawdata);
}

//{ == }であることを確かめる
static bool	countParentheses(std::string rawdata, std::string start, std::string end)
{
	size_t	num_of_start = 0;
	size_t	num_of_end = 0;
	size_t	offset = 0;	
	long	pos = 0;

	while (1)
	{
		pos = rawdata.find(start, offset);
		if ((unsigned long)pos == std::string::npos)
			break;
		num_of_start ++;
		offset = pos + start.length();
	}

	offset = 0;
	pos = 0;

	while (1)
	{
		pos = rawdata.find(end, offset);
		if ((unsigned long)pos == std::string::npos)
			break;
		num_of_end ++;
		offset = pos + end.length();
	}

	std::cout << YELLOW "num_of_{}: " << num_of_end << RESET << std::endl;

	if(num_of_start == num_of_end)
		return (true);
	else
		return (false);
}

//第1層をパース
static std::vector<std::string>	cutPorts(std::string rawdata, std::string start, std::string end)
{
	std::vector<std::string>	list;
	long			pos;
	const int		startlen = start.length();
	const int		endlen = end.length();
	int				offset;
	int				depth;
	int				foundstart;
	int				foundend;

	if (startlen == 0 || endlen == 0)
		list.push_back(rawdata);
	else{
		offset = 0;
		pos = 0;
		depth = 0;

		while (1)
		{
			foundstart = rawdata.find(start, offset);
			foundend = rawdata.find(end, offset);
			
			if (foundstart <= foundend)
			{
				if ((unsigned long)foundstart == std::string::npos)
				{
					list.push_back(rawdata.substr(pos, foundstart - pos));
					break ;
				}
				depth ++;
				offset = foundstart + startlen;
			}
			else if (foundend < foundstart)
			{
				if ((unsigned long)foundend == std::string::npos)
				{
					list.push_back(rawdata.substr(pos, foundend + endlen - pos));
					break ;
				}
				depth --;
				offset = foundend + endlen;
				if (depth == 0)
				{
					list.push_back(rawdata.substr(pos, foundend + endlen - pos));
					pos = foundend + endlen;
				}
			}
		}
	}
	return (list);
}

Result<std::vector<std::string>, bool>	makeConf(std::string filepath)
{
	bool						status;
	std::string					rawdata;
	std::vector<std::string>	firstlayer;

	Result<std::string, bool>res = openAndReadConf(filepath);	
	
	if (res.isError() == true)
		return Error<bool>(false);
	std::cout << YELLOW "open and read success" RESET << std::endl;

	rawdata = res.getOk();

	//なんとかして・・・パースする・・・

		//カッコの数を数えて{==}でなければ弾く
		status = countParentheses(rawdata, "{", "}");				
		if (status == false)
			return Error<bool>(false);
		std::cout << YELLOW "count parent success" RESET << std::endl;

		//第一層の頭とカッコを数え、その中身をVectorに切り出す
		firstlayer = cutPorts(rawdata, "{", "}");

		std::cout << YELLOW << firstlayer.size() << RESET << std::endl;
		for(size_t i = 0; i < firstlayer.size(); i ++)
			std::cout << YELLOW "vector " << i << ": " << firstlayer.at(i) << RESET << std::endl;

	//終わり
	return Ok<std::vector<std::string> >(firstlayer);
}

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

static int	readListen(Config &conf, std::string	oneline)
{
	size_t	pos;

	status = oneline.find(":", 0);
	if (status == std::string::npos)
	{
		std::stringstream	ss;
		int					port;
		
		if (oneline.size() != 2 && oneline.size() != 3)
		{
			return (-1);
		}
		ss << oneline.at(1);
		ss >> port;
		if (!(1 <= port && port <= 65535)
			return (-1);
		conf.setPort(port);
	}
	else
	{
		std::vector<std::string>	div;
		int							port;
		std::stringstream			ss;

		if (oneline.size() != 2 && oneline.size() != 3)
			return (-1);
		div = lineSpliter(oneline.at(1), ":");
		
	}
}

static int	checkSettings(Config conf, std::string	oneline)
{
	std::vector<std::string>	lines;

	lines = lineSpliter(oneline, " ");
	for (size_t i = 0; i < lines.size(); i ++)
	{
		std::cout << YELLOW ": " << lines.at(i) << RESET << std::endl;		
		
		if(lines.at(i).empty == false)
		{
			if (lines.at(i) == "listen")
			{
				if (i + 1 >= lines.size())	
					return (-1);	
				
			}
			else if (lines.at(i) == "server_name")
			{}
			else if (lines.at(i) == "root")
			{}
			else if (lines.at(i) == "location")
			{}
			else if (lines.at(i) == "error_page")
			{}
			else if (lines.at(i) == "return")
			{}
		}

		//ヘッダーが正しいか判別
		//空行は飛ばす
		//内容があれば詰める
	}
	conf.setPort(oneline.size());
	return 0;
}

static Config	getConf(std::string raw)
{
	std::vector<std::string>	lines;
	std::vector<std::string>	oneline;
	Config						conf;

	//delete&ignore comments
	cutOffStr(raw, "#", "\n");

	//interpret spaces as " "
	replaceStr(raw, "\n", " ");
	replaceStr(raw, "\r", " ");
	replaceStr(raw, "\t", " ");
	replaceStr(raw, "\v", " ");
	replaceStr(raw, "\f", " ");
	replaceStr(raw, "{", ";{;");
	replaceStr(raw, "}", ";};");

	//cut string by ";"
	lines = lineSpliter(raw, ";");

	//get confs
	for (size_t i = 0; i < lines.size(); i ++)
	{
		checkSettings(conf, lines.at(i));
		std::cout << RED "/" << RESET << std::endl;
	}
	return (conf);
}

Result<std::vector<Config>, bool>	parsePortVecs(std::vector<std::string> list)
{
	std::vector<Config>	confs;
	Config				tmp;
	int					status;

	//リストの数だけループする

	for (size_t i = 0; i < list.size(); i ++)
	{
		//頭が”Server”であることを確認 そうでなければスキップ
		status = isServerSetting(list.at(i));

		if (status != -1)
		{			
			//中身を読みだしていく
			tmp = getConf(list.at(i));
			confs.push_back(tmp);
		}
	}
	
	//返す
	return Ok<std::vector<Config> >(confs);
}

int main()
{
	std::vector<std::string>	list;

	Result<std::vector<std::string>, bool> res = makeConf("../conf_files/test.conf");
	if (res.isOK() == true)
		list = res.getOk();
	parsePortVecs(list);
	
	return 0;
}


