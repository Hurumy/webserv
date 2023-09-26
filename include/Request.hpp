/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/26 11:03:56 by shtanemu         ###   ########.fr       */
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
		bool				setUrl(std::string _url);
		std::string const	getUrl(void) const;
		bool				setMethod(std::string _method);
		std::string const	getMethod(void) const;
		const tag &getPhase() const;
		bool loadPayload(CSocket &csocket);
		bool loadHeader(CSocket &csocket);
	protected:
	private:
		std::string	url;
		std::string method;
		std::size_t contentLength;
		std::size_t lastContentLength;
		bool isCompleteHeader;
		tag phase;

		static std::vector<std::string> initMethods();
		bool loadRequestLine(CSocket &csocket);
		bool loadHeader(CSocket const &csocket);
		bool isMethod(std::string const &word);
		bool isValidURL(std::string const &word);
		bool isVersion(std::string const &word);
};
