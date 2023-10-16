/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 12:28:43 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/16 14:28:53 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"

class Status {
	private:
		static std::map<unsigned int, std::string> const statusMap;
		static std::map<unsigned int, std::string> initStatusMap();
	protected:
	public:
		static Result<std::string, bool> const getStatusMessage(unsigned int const statusCode);
};