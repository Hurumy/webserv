/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 15:27:34 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/07 12:24:11 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Address.hpp"
#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "SSocket.hpp"
#include "webserv.hpp"
#include "ft.hpp"

Result<std::vector<std::string>, bool> cutOutLocation(
	const std::vector<std::string> &origin, Config &conf);

int l_readLocation(Location &loc, std::string oneline);
int l_readRoot(Location &loc, std::string oneline);
int l_readErrorPage(Location &loc, std::string oneline);
int l_readReturn(Location &loc, std::string oneline);
int l_readRewrite(Location &loc, std::string oneline);
int l_readAutoindex(Location &loc, std::string oneline);
int l_readMaxBodySize(Location &loc, std::string oneline);
int l_readIndex(Location &loc, std::string oneline);
int l_readUploadPath(Location &loc, std::string oneline);
int l_readAllowedMethods(Location &loc, std::string oneline);
int l_readCGIExtension(Location &loc, std::string oneline);
int l_readAlias(Location &loc, std::string oneline);

std::vector<std::string> lineSpliter(std::string origin, std::string delim);
Result<std::vector<std::string>, bool> cutConfByDirective(std::string filepath);
std::vector<std::string> cutConfToEachPort(std::string raw);
Result<Config, bool> parsePortVecs(std::string port);
std::string replaceStr(std::string &origin, std::string search,
					   std::string replace);
Result<std::vector<Config>, bool> parseConf(std::string filepath);

int errorInInit(std::string errormessage);

int readListen(Config &conf, std::string oneline);
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
int readCGIExtension(Config &conf, std::string oneline);

int thereisnoListen(Config &conf);

bool isNumber(std::string str);
bool isSpace(std::string str);
bool isCapitalAlphabet(std::string str);
bool isSmallAlphabet(std::string str);
bool isAlphabet(std::string str);
bool isAlnum(std::string str);
