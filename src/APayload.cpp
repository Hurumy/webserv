/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:15:57 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/03 16:37:49 by shtanemu         ###   ########.fr       */
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

std::string const &APayload::getMetadata(std::string const &key) const
{	
	if (metadata.find(key) == metadata.end())
		return (metadata.rbegin()->second);
	else
		return (metadata.at(key));
}

bool APayload::addMetadata(std::string const &key, std::string const &value)
{
	if (metadata.find(key) != metadata.end())
		return (false);
	else
		metadata[key] = value;
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
