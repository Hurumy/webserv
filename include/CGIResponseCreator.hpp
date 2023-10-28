/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:36:35 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/28 17:11:19 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CSocket.hpp"
#include "MetaVariables.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define WS_CGI_VERSION "CGI/1.1"
#define WS_HTTP_VERSION "HTTP/1.1"

class CGIResponseCreator {
	public:
		enum tag { CGISTARTUP, CGIWRITE, CGIRECV, CGILASTRECV, CGIFIN };
		enum responseType { DOC, LOCALREDIR, CLIENTREDIR, OTHER };
		CGIResponseCreator(Request &_request, Response &_response,
						   const std::string &_cgiPath);

		CGIResponseCreator::tag const &getPhase() const;
		void setPhase(CGIResponseCreator::tag const &_phase);
		pid_t getPid() const;
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
		pid_t waitChildProc();
		bool setCGIOutput();
		bool deinit();
		bool setEnvVars();

	protected:
	private:
		char **_createEnvp();
		char **_createArgv();
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
		bool _setServerName();
		bool _setServerPort();
		bool _setServerProtocol();
		bool _setRuntime();
		CGIResponseCreator::responseType _loadCGIReponse();

		Request &request;
		Response &response;
		CGIResponseCreator::tag phase;
		pid_t pid;
		int wstatus;
		int inpfd[2];
		int outpfd[2];
		int monitoredfd;
		short revents;
		std::string runtimePath;
		std::string cgiOutput;
		std::string cgiPath;
		std::string hostName;
		int portNum;
		MetaVariables metaVariables;
};
