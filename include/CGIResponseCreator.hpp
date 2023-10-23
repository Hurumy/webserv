/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:36:35 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/23 18:22:46 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "MetaVariables.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define WS_CGI_VERSION "CGI/1.1"
#define WS_HTTP_VERSION "HTTP/1.1"

class CGIResponseCreator {
	public:
		enum tag { CGISTARTUP, CGIWRITE, CGIRECV, CGIRECVFIN };
		CGIResponseCreator(Request &_request, Response &_response, const std::string &_cgiPath);

		CGIResponseCreator::tag const &getPhase() const;
		void setPhase(CGIResponseCreator::tag const &_phase);
		short getRevents() const;
		void setRevents(short const _revents);
		void setPortNum(int const _portNum);
		int getPortNum() const;
		void setHostName(std::string const &_hostName);
		std::string const &getHostName() const;
		bool execCGIScript();
		int getMonitoredfd() const;
		void setMonitoredfd(CGIResponseCreator::tag const &_phase);
		bool writeMessageBody() const;
		bool recvCGIOutput();
		bool setCGIOutput();
		bool deinit();
		bool setEnvVars();

	protected:
	private:
		char **_createEnvp();
		bool _setAuthType();
		bool _setContentLength();
		bool _setContentType();
		bool _setGateWayInterface();
		bool _setPathInfo();
		bool _setPathTranslated();
		bool _setQuerySring();
		bool _setRemoteAddr();
		bool _setRequestMethod();
		bool _setScriptName();
		bool _setServerProtocol();

		Request &request;
		Response &response;
		CGIResponseCreator::tag phase;
		int inpfd[2];
		int outpfd[2];
		int monitoredfd;
		short revents;
		std::string cgiOutput;
		std::string cgiPath;
		std::string hostName;
		int portNum;
		MetaVariables metaVariables;
};
