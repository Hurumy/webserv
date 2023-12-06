/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/26 11:16:17 by shtanemu         ###   ########.fr       */
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
		enum tag { REQLINE, HEADER, BODY, CHUNKEDBODY };
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
		void setRemoteAddr(std::string const &_remoteAddr);
		std::string const &getRemoteAddr() const;
		void setLocalAddr(std::string const &_localAddr);
		std::string const &getLocalAddr() const;
		void setLocalPort(unsigned int const _localPort);
		unsigned int getLocalPort() const;
		void countUpCntCGIExec();
		std::size_t getCntCGIExec() const;

	protected:
	private:
		std::string url;
		std::string method;
		std::size_t contentLength;
		std::size_t lastContentLength;
		tag phase;
		std::string remoteAddr;
		std::string localAddr;
		unsigned int localPort;
		std::size_t cntCGIExec;
		std::size_t chunkLength;
		std::string chunkExt;

		static std::vector<std::string> initMethods();
		bool loadRequestLine(CSocket &csocket);
		bool loadHeader(CSocket const &csocket);
		bool isMethod(std::string const &word);
		bool isValidURL(std::string const &word);
		bool isVersion(std::string const &word);
};
