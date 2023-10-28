/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APayload.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:08:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/28 17:01:51 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Result.hpp"
#include "webserv.hpp"

class APayload {
	private:
	protected:
		std::string version;
		std::string body;
		std::map<std::string, std::string> header;

	public:
		virtual ~APayload();

		virtual const std::string getLines() const = 0;
		bool setVersion(std::string const &_version);
		std::string const &getVersion() const;
		bool setBody(std::string const &_body);
		std::string const &getBody() const;
		bool addHeader(std::string const &key, std::string const &value);
		bool setHeader(std::string const &key, std::string const &value);
		bool addBody(std::string const &_body);
		Result<std::string, bool> const getHeader(std::string const &key) const;
		std::map<std::string, std::string> const &getAllHeader() const;
};
