/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/18 00:42:33 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>

#include "CSocket.hpp"
#include "webserv.hpp"
#include "APayload.hpp"


class Request: public virtual APayload
{
	public:
		enum tag {
			REQLINE,
			HEADER,
			BODY,
		};
		Request();

		const std::string	getLines() const;
		bool				setUrl(std::string url);
		std::string const	getUrl(void) const;
		bool				setMethod(std::string method);
		std::string const	getMethod(void) const;
		const tag &getPhase() const;
		bool loadPayload(CSocket &csocket);
		bool loadHeader(CSocket &csocket);
	protected:
	private:
		std::string	url;
		std::string method;
		std::size_t contentLength;
		bool isCompleteHeader;
		static std::vector<std::string> const methods;
		tag phase;

		static std::vector<std::string> initMethods();
		bool setRequestLine(std::string const &line);
		bool loadRequestLine(CSocket &csocket);
		bool setHeader(std::string const &line);
		bool loadHeader(CSocket const &csocket);
		bool isMethod(std::string const &word);
		bool isURL(std::string const &word);
		bool isVersion(std::string const &word);
};
