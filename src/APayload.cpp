/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:15:57 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/28 17:02:02 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

APayload::~APayload() {}

bool APayload::setVersion(std::string const &_version) {
	version = _version;
	return (true);
}

bool APayload::setBody(std::string const &_body) {
	body = _body;
	return (true);
}

bool APayload::addBody(std::string const &_body) {
	body.append(_body);
	return (true);
}

Result<std::string, bool> const APayload::getHeader(
	std::string const &key) const {
	if (header.empty() == true) return Error<bool>(false);
	if (header.find(key) == header.end())
		return Error<bool>(false);
	else
		return Ok<std::string>(header.at(key));
}

bool APayload::addHeader(std::string const &key, std::string const &value) {
	return header.insert(std::make_pair(key, value)).second;
}

bool APayload::setHeader(std::string const &key, std::string const &value) {
	header[key] = value;
	return true;
}

std::string const &APayload::getVersion() const { return (version); }

std::string const &APayload::getBody() const { return (body); }

std::map<std::string, std::string> const &APayload::getAllHeader() const {
	return header;
}
