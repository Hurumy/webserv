/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:42:17 by komatsud          #+#    #+#             */
/*   Updated: 2023/12/26 20:25:59 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include <sstream>

const std::string Response::getLines() const {
	std::string line;
	std::ostringstream numStatus;
	std::ostringstream numContentLength;

	line += version + " ";
	numStatus << status;
	line += numStatus.str() + " ";
	line += statusMessage + "\r\n";
	for (std::map<std::string, std::string>::const_iterator iter =
			 header.begin();
		 iter != header.end(); ++iter) {
		line += iter->first + ": ";
		line += iter->second + "\r\n";
	}
	line += "\r\n";
	line += body;
	return (line);
}

bool Response::setStatus(unsigned int _status) {
	status = _status;
	return (true);
}

bool Response::setStatusMessage(std::string const &message) {
	statusMessage = message;
	return (true);
}

unsigned int Response::getStatus() const { return (status); }

std::string const &Response::getStatusMessage() const {
	return (statusMessage);
}

void Response::addSetCookie(std::string const &cookie) {
	setCookies.push_back(cookie);
}

std::string Response::getAllSetCookies() const {
	std::string lines;

	for (std::list<std::string>::const_iterator iter = setCookies.begin(); iter != setCookies.end(); ++iter) {
		lines.append(*iter);
		lines.append("\r\n");
	}
	return lines;
}