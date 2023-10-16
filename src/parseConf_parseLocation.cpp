/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConf_parseLocation.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 10:37:32 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 14:42:35 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

static int checkLocationSettings(Location &res, std::string str)
{
	int							status = 0;
	std::vector<std::string>	lines;

	//std::cout << str << std::endl;

	lines = lineSpliter(str, " ");
	for (size_t i = 0; i < lines.size(); i++)
	{
		//std::cout << YELLOW ": " << lines.at(i) << "|" << RESET << std::endl;

		if (lines.at(i).empty() == false)
		{
			if (lines.at(i) == "location") {
				status = l_readLocation(res, str);
				break ;
			} else if (lines.at(i) == "root") {
				status = l_readRoot(res, str);
				break ;
			} else if (lines.at(i) == "error_page") {
				status = l_readErrorPage(res, str);
				break;
			} else if (lines.at(i) == "return") {
				status = l_readReturn(res, str);
				break;
			} else if (lines.at(i) == "rewrite") {
				status = l_readRewrite(res, str);
				break;
			} else if (lines.at(i) == "autoindex") {
				status = l_readAutoindex(res, str);
				break;
			} else if (lines.at(i) == "client_max_body_size") {
				status = l_readMaxBodySize(res, str);
				break;
			} else if (lines.at(i) == "index") {
				status = l_readIndex(res, str);
				break;
			} else if (lines.at(i) == "upload_path") {
				status = l_readUploadPath(res, str);
				break;
			} else if (lines.at(i) == "allowedMethods") {
				status = l_readAllowedMethods(res, str);
				break;
			} else if (lines.at(i) == "cgi_extension") {
				status = l_readCGIExtension(res, str);
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

Result<int, bool>	parseLocation(std::vector<std::string> const &line, Config &conf)
{
	Location	res;
	std::string	path;
	int			status = 0;

	for (size_t i = 0; i < line.size(); i ++)
	{
		if (line.at(i).empty() == false)
		{
			status = checkLocationSettings(res, line.at(i));
			if (status == -1)
			{
				return Error<bool>(false);
			}
		}
	}

	path = res.getUri();
	conf.addLocations(path, res);

	return Ok<int>(0);
}

Result<std::vector<std::string>, bool>	cutOutLocation(const std::vector<std::string> &origin, Config &conf)
{
	size_t	hed_pos;			 // "location /"
	size_t	st_pos; 			 //  "{"
	size_t	end_pos;			 //  "}"
	std::vector<std::string>	line;
	std::vector<std::string>	tmp;
	std::vector<std::string>	loc;


	//originをLineにコピー
	for (size_t a = 0; a < origin.size(); a ++)
	{
		line.push_back(origin.at(a));
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
						tmp.erase(tmp.begin(), tmp.end());
						
						//tmpにコピーしてLineに戻す
						for (size_t k = 0; k < line.size(); k ++)
						{
							if (k < i || k > j)
							{
								tmp.push_back(line.at(k));
							}
							else
							{
								loc.push_back(line.at(k));
							}
						}

						//Locationパーサーにかけ、解釈し、Configに詰める
						Result<int, bool> res_1 = parseLocation(loc, conf);
						if (res_1.isOK() == false)
						{
							return Error<bool>(false);
						}
						

						//lineにtmpをコピーする
						line.erase(line.begin(), line.end());
						for (size_t k = 0; k < tmp.size(); k ++)
						{
							line.push_back(tmp.at(k));
						}
					}
				}
			}
		}
	}

	//Locationディレクティブの部分を切り取った文字列を返す
	return Ok<std::vector<std::string> >(line);
}


