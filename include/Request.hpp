/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/12 19:58:49 by shtanemu         ###   ########.fr       */
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
	void setPhase(Request::tag _phase);
	bool loadPayload(CSocket &csocket);
	bool loadHeader(CSocket &csocket);
	int getMonitoredfd() const;
	void setMonitoredfd(Request::tag _phase);
	short getRevents() const;
	void setRevents(short const _revents);
	bool setEnvVars() const;
	bool execCGIScript();
	bool writeMessageBody() const;
	bool recvCGIOutput() const;

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
	int monitoredfd;
	short revents;

	static std::vector<std::string> initMethods();
	bool loadRequestLine(CSocket &csocket);
	bool loadHeader(CSocket const &csocket);
	bool isMethod(std::string const &word);
	bool isValidURL(std::string const &word);
	bool isVersion(std::string const &word);
};
