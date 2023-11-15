/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:36:35 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/08 13:27:25 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <sstream>
#include <string>

#include "CSocket.hpp"
#include "Config.hpp"
#include "MetaVariables.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"
#include "Response.hpp"

#define WS_CGI_VERSION "CGI/1.1"
#define WS_HTTP_VERSION "HTTP/1.1"
#define SCHEMENAME_FOLLOWED_BY_COLON_HTTP "http:"
#define SCHEMENAME_FOLLOWED_BY_COLON_HTTPS "https:"

class CGIResponseCreator {
	public:
		enum tag {
				CGIREADY,
				CGISTARTUP,
				CGIWRITE,
				CGIRECV,
				CGILASTRECV,
				CGIFIN
		};
		enum type { DOC, LOCALREDIR, CLIENTREDIR, OTHER };
		CGIResponseCreator(Request &_request, Response &_response,
						   const std::string &_cgiPath);

		CGIResponseCreator::tag const &getPhase() const;
		void setPhase(CGIResponseCreator::tag const &_phase);
		CGIResponseCreator::type const &getResponseType() const;
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
		bool writeMessageBody();
		bool recvCGIOutput();
		pid_t waitChildProc();
		bool setCGIOutput(std::vector<Config> const &configs);
		bool deinit();
		bool setEnvVars();
		void setCGIPath(const std::string &_cgiPath);

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
		bool _chDirectory();
		bool _setDocumentRedirResponse(std::istringstream &issline,
									   std::string &line,
									   std::istringstream &issheader,
									   std::string &key);
		bool _setLocalRedirResponse(std::istringstream &issline,
									std::string &location);
		bool _setClientRedirResponse(std::istringstream &issline,
									 std::string &line,
									 std::istringstream &issheader,
									 std::string &location);
		void _setCGIErrorResponse(std::vector<Config> const &configs);
		bool _deleteVariables(char **envp, char **argv);

		Request &request;
		Response &response;
		CGIResponseCreator::tag phase;
		CGIResponseCreator::type responseType;
		pid_t pid;
		int wstatus;
		int inpfd[2];
		int outpfd[2];
		int monitoredfd;
		short revents;
		std::string runtimePath;
		std::string cgiIntput;
		std::string cgiOutput;
		std::string cgiPath;
		std::string hostName;
		int portNum;
		MetaVariables metaVariables;
		std::time_t startTime;
};
