/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:08:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/03 17:03:44 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class APayload
{
	private:
	protected:
		std::string 						version;
		std::string							body;
		std::map<std::string, std::string> 	header;
	public:
		virtual const std::string			getLines() const = 0;
		bool								setVersion(std::string const &_version);
		std::string const					&getVersion() const;
		bool								setBody(std::string const &_body);
		std::string	const					&getBody() const;
		bool								addHeader(std::string const &key, std::string const &value);
		std::string const					&getHeader(std::string const &key) const;
};
