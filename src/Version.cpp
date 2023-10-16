/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Version.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:58:27 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/25 19:15:22 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Version.hpp"

#include <algorithm>

#include "Error.hpp"
#include "Ok.hpp"

const std::vector<std::string> Version::versions = Version::createVersions();

std::vector<std::string> Version::createVersions() {
	std::vector<std::string> _versions;
	_versions.push_back("HTTP/1.1");
	return _versions;
}

Result<std::string, bool> Version::getVersion(std::string const &version) {
	if (versions.empty() == true) {
		return Error<bool>(false);
	}
	std::vector<std::string>::const_iterator iter =
		std::find(versions.begin(), versions.end(), version);
	if (iter == versions.end()) {
		return Error<bool>(false);
	}
	return Ok<std::string>(*iter);
}
