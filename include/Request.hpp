/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/11 17:27:00 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

#include "APayload.hpp"
#include "CSocket.hpp"
#include "webserv.hpp"

class Request : public virtual APayload {
   public:
	enum tag {
		REQLINE,
		HEADER,
		BODY,
		CGISTARTUP,
		CGIWRITE,
		CGIRECV,
	};
	Request();

	const std::string getLines() const;
	bool setUrl(std::string _url);
	std::string const getUrl(void) const;
	bool setMethod(std::string _method);
	std::string const getMethod(void) const;
	const tag &getPhase() const;
	bool loadPayload(CSocket &csocket);
	bool loadHeader(CSocket &csocket);
	int const *getOutpfd() const;
	short getRevents() const;
	void setRevents(short const _revents);

   protected:
   private:
	std::string url;
	std::string method;
	std::size_t contentLength;
	std::size_t lastContentLength;
	bool isCompleteHeader;
	tag phase;
	int inpfd[2];
	int outpfd[2];
	short revents;

	static std::vector<std::string> initMethods();
	bool loadRequestLine(CSocket &csocket);
	bool loadHeader(CSocket const &csocket);
	bool isMethod(std::string const &word);
	bool isValidURL(std::string const &word);
	bool isVersion(std::string const &word);
};
