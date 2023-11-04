/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:08:04 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/04 11:29:47 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Result<std::string, bool> const Header::getHeader(std::string const &_key) const {
	if (header.empty() == true) return Error<bool>(false);
	if (header.find(_key) == header.end())
		return Error<bool>(false);
	else
		return Ok<std::string>(header.at(_key));
}

bool Header::addHeader(std::string const &key, std::string const &value) {
	return header.insert(std::make_pair(key, value)).second;
}

bool Header::setHeader(std::string const &key, std::string const &value) {
	header[key] = value;
	return true;
}

std::map<std::string, std::string, sComp> const &Header::getAllHeader() const {
	return header;
}


