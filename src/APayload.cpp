/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:15:57 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/03 17:03:44 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"

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

std::string const &APayload::getHeader(std::string const &key) const
{	
	if (header.find(key) == header.end())
		return (header.rbegin()->second);
	else
		return (header.at(key));
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
