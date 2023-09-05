/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:32:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/05 13:02:01 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Status.hpp"

const std::map<unsigned int, std::string> Status::statusMap = initStatusMap();

std::map<unsigned int, std::string> Status::initStatusMap() {
	std::map<unsigned int, std::string> m;
	m[100] = "Continue";
	m[101] = "Switching Protocol";
	m[102] = "Processing";
	m[103] = "Early Hints";
	m[200] = "OK";
	m[201] = "Created";
	m[202] = "Accepted";
	m[203] = "Non-Authoritative Information";
	m[204] = "No Content";
	m[205] = "Reset Content";
	m[206] = "Partial Content";
	m[300] = "Multiple Choice";
	m[301] = "Moved Permanently";
	m[302] = "Found";
	m[304] = "Not Modified";
	m[400] = "Bad Request";
	m[401] = "Unauthorized";
	m[402] = "Payment Required";
	m[403] = "Forbidden";
	m[404] = "Not Found";
	m[405] = "Method Not Allowed";
	m[406] = "Not Acceptable";
	m[407] = "Proxy Authentication Required";
	m[408] = "Request Timeout";
	m[409] = "Conflict";
	m[410] = "Gone";
	m[411] = "Length Required";
	m[412] = "Precondition Failed";
	m[413] = "Payload Too Large";
	m[414] = "URI Too Long";
	m[415] = "Unsupported Media Type";
	m[416] = "Range Not Satisfiable";
	m[417] = "Expectation Failed";
	m[422] = "Unprocessable Entity";
	m[423] = "Locked";
	m[425] = "Too Early";
	m[426] = "Upgrade Required";
	m[429] = "Too Many Requests";
	m[431] = "Request Header Fields Too Large";
	m[500] = "Internal Server Error";
	m[501] = "Not Implemented";
	m[502] = "Bad Gateway";
	m[503] = "Service Unavailable";
	m[504] = "Gateway Timeout";
	m[505] = "HTTP Version Not Supported";
	m[506] = "Variant Also Negotiates";
	m[507] = "Insufficient Storage";
	m[508] = "Loop Detected";
	m[510] = "Not Extended";
	m[511] = "Network Authentication Required";
	return m;
}

Result<std::string, bool> const Status::getStatusMessage(
	unsigned int const statusCode) {
	if (statusMap.empty() == true) return Error<bool>(false);
	if (statusMap.find(statusCode) == statusMap.end())
		return Error<bool>(false);
	else
		return Ok<std::string>(statusMap.at(statusCode));
}
