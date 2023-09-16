/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:27:34 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/15 18:11:00 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

std::vector<std::string> lineSpliter(std::string origin, std::string delim);
Result<std::vector<std::string>, bool>	cutConfByDirective(std::string filepath);
std::vector<std::string>	cutConfToEachPort(std::string raw);
Result<Config, bool>	parsePortVecs(std::string port);
std::string	replaceStr(std::string &origin, std::string search, std::string replace);
Result<std::vector<Config>, bool>	parseConf(std::string filepath);

int	errorInInit(std::string errormessage);

int	readListen(Config &conf, std::string oneline);
int readServerName(Config &conf, std::string oneline);
int readRoot(Config &conf, std::string oneline);
int readErrorPage(Config &conf, std::string oneline);
int readRewrite(Config &conf, std::string oneline);
int readReturn(Config &conf, std::string oneline);
int readAutoindex(Config &conf, std::string oneline);
int readMaxBodySize(Config &conf, std::string oneline);
int readIndex(Config &conf, std::string oneline);
int readUploadPath(Config &conf, std::string oneline);
int readAllowedMethods(Config &conf, std::string oneline);

bool	isNumber(std::string str);
bool	isSpace(std::string str);
bool	isCapitalAlphabet(std::string str);
bool	isSmallAlphabet(std::string str);
bool	isAlphabet(std::string str);
bool	isAlnum(std::string str);
