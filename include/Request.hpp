/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/26 11:10:21 by shtanemu         ###   ########.fr       */
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
		std::string const &getUrl() const;
		bool setMethod(std::string _method);
		std::string const &getMethod() const;
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
		void setRemoteAddr(std::string const &_remoteAddr);
		std::string const &getRemoteAddr() const;
		void setLocalAddr(std::string const &_localAddr);
		std::string const &getLocalAddr() const;
		void setLocalPort(unsigned int const _localPort);
		unsigned int getLocalPort() const;

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
		std::string remoteAddr;
		std::string localAddr;
		unsigned int localPort;

		static std::vector<std::string> initMethods();
		bool loadRequestLine(CSocket &csocket);
		bool loadHeader(CSocket const &csocket);
		bool isMethod(std::string const &word);
		bool isValidURL(std::string const &word);
		bool isVersion(std::string const &word);
};
