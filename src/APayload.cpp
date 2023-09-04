/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:15:57 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/04 15:45:40 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

bool APayload::setVersion(std::string const &_version)
{
	version = _version;
	return (true);
}

bool APayload::setBody(std::string const &_body)
{
	body = _body;
	return (true);
}

Result<std::string, bool> const APayload::getHeader(std::string const &key) const
{	
	if (header.find(key) == header.end())
		return Error<bool>(false);
	else
		return Ok<std::string>(header.at(key));
}

bool APayload::addHeader(std::string const &key, std::string const &value)
{
	if (header.find(key) != header.end())
		return (false);
	else
		header[key] = value;
	return (true);
}

std::string const &APayload::getVersion() const
{
	return(version);
}

std::string const &APayload::getBody() const
{
	return (body);
}
