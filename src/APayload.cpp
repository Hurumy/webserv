/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:15:57 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/04 11:23:50 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"

#include <algorithm>
#include <cctype>

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

APayload::~APayload(){}

bool APayload::setVersion(std::string const &_version) {
	version = _version;
	return (true);
}

bool APayload::setBody(std::string const &_body) {
	body = _body;
	return (true);
}

Result<std::string, bool> const APayload::getHeader(
	std::string const &_key) const {
	std::string const tmp = toLower(_key);
	// std::cout << "tmp: " << tmp.size() << ": " << tmp << std::endl;
	if (header.empty() == true) return Error<bool>(false);
	if (header.find(tmp) == header.end())
		return Error<bool>(false);
	else
		return Ok<std::string>(header.at(tmp));
}

bool APayload::addHeader(std::string const &key, std::string const &value) {
	std::string const tmp = toLower(key);
	return header.insert(std::make_pair(tmp, value)).second;
}

bool APayload::setHeader(std::string const &key, std::string const &value) {
	std::string const tmp = toLower(key);
	header[tmp] = value;
	return true;
}

std::string const &APayload::getVersion() const { return (version); }

std::string const &APayload::getBody() const { return (body); }

std::map<std::string, std::string> const &APayload::getAllHeader() const {
	return header;
}
