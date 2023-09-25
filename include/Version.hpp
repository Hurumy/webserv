/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Version.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:32:26 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/25 19:13:45 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include "Result.hpp"

class Version {
	private:
		static const std::vector<std::string> versions;
	protected:
	public:
		static Result<std::string, bool> getVersion(std::string const &version);
		static std::vector<std::string> createVersions();
};
