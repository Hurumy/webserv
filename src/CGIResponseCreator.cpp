/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:54:44 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/18 15:51:04 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIResponseCreator.hpp"

#include <cstring>
#include <algorithm>

#include "Result.hpp"
#include "puterror.hpp"

CGIResponseCreator::CGIResponseCreator(Request &_request, Response &_response, const std::string &_cgiPath)
	: request(_request),
	  response(_response),
	  phase(CGIResponseCreator::CGISTARTUP),
	  monitoredfd(0),
	  revents(0),
	  cgiPath(_cgiPath) {
	std::memset(inpfd, 0, sizeof(inpfd));
	std::memset(outpfd, 0, sizeof(outpfd));
}

CGIResponseCreator::tag const &CGIResponseCreator::getPhase() const {
	return phase;
}

void CGIResponseCreator::setPhase(CGIResponseCreator::tag const &_phase) {
	phase = _phase;
}

short CGIResponseCreator::getRevents() const { return revents; }

void CGIResponseCreator::setRevents(short const _revents) {
	revents = _revents;
}

bool CGIResponseCreator::_setAuthType() {
	Result<std::string, bool> result(request.getHeader("Authorization"));
	std::string authScheme;

	if (result.isError() == true) {
		return false;
	}
	std::istringstream iss(result.getOk());
	iss >> authScheme;
	metaVariables.setMetaVar(MetaVariables::AUTH_TYPE, authScheme);
	return true;
}

bool CGIResponseCreator::_setContentLength() {
	std::stringstream ss;

	ss << request.getBody().size();
	metaVariables.setMetaVar(MetaVariables::CONTENT_LENGTH, ss.str());
	return true;
}

bool CGIResponseCreator::_setGateWayInterface() {
	metaVariables.setMetaVar(MetaVariables::GATEWAY_INTERFACE, WS_CGI_VERSION);
	return true;
}

bool CGIResponseCreator::_setPathInfo() {
	std::string filename;
	std::string postFilename;
	std::size_t posCut;

	filename = cgiPath.substr(cgiPath.rfind("/"));
	postFilename = request.getUrl().substr(request.getUrl().find(filename) + filename.size());
	posCut = std::min(postFilename.find("?"), postFilename.find("#"));
	metaVariables.setMetaVar(MetaVariables::PATH_INFO, postFilename.substr(0, posCut));
	return true;
}

bool CGIResponseCreator::_setPathTranslated() {
	std::string pathInfo;

	pathInfo = metaVariables.getMetaVar(MetaVariables::PATH_INFO);
	metaVariables.setMetaVar(MetaVariables::PATH_TRANSLATED, cgiPath + pathInfo);
	return true;
}

bool CGIResponseCreator::_setQuerySring() {
	std::string filename;
	std::string postFilename;
	std::size_t posQueryString;
	std::size_t posHash;

	filename = cgiPath.substr(cgiPath.rfind("/"));
	postFilename = request.getUrl().substr(request.getUrl().find(filename) + filename.size());
	posQueryString = postFilename.find("?");
	if (posQueryString == std::string::npos) return false;
	posHash = postFilename.find("#");
	if (posHash == std::string::npos || posQueryString > posHash) {
		metaVariables.setMetaVar(MetaVariables::QUERY_STRING, postFilename.substr(posQueryString + 1));
	} else {
		metaVariables.setMetaVar(MetaVariables::QUERY_STRING, postFilename.substr(posQueryString + 1, posHash));
	}
	return true;
}

bool CGIResponseCreator::_setRemoteAddr() {
	metaVariables.setMetaVar(MetaVariables::REMOTE_ADDR, request.getRemoteAddr());
	return true;
}

bool CGIResponseCreator::_setRequestMethod() {
	metaVariables.setMetaVar(MetaVariables::REQUEST_METHOD, request.getMethod());
	return true;
}

bool CGIResponseCreator::_setServerProtocol() {
	metaVariables.setMetaVar(MetaVariables::SERVER_PROTOCOL, WS_HTTP_VERSION);
	return true;
}

// server hostname 取れるように
// server port 取れるように
// AUTH_TYPE, CONTENT_LENGTH, CONTENT_TYPE, SERVER_NAME, SERVER_PORT
bool CGIResponseCreator::setEnvVars() {
	// For develope
	request.getPhase();
	_setAuthType();
	_setGateWayInterface();
	_setPathInfo();
	_setPathTranslated();
	_setQuerySring();
	_setRemoteAddr();
	_setRequestMethod();
	return true;
}

bool CGIResponseCreator::execCGIScript() {
	extern char **environ;

	if (pipe(inpfd) == -1) {
		// error handling
	}
	if (pipe(outpfd) == -1) {
		// error handling
	}
	pid_t const pid = fork();
	if (pid == -1) {
		// error handling
	}
	if (pid == 0) {
		// for developement
		char *const argv[] = {strdup("/bin/ls"), NULL};
		dup2(inpfd[0], 0);
		dup2(outpfd[1], 1);
		close(inpfd[0]);
		close(inpfd[1]);
		close(outpfd[0]);
		close(outpfd[1]);
		execve("/bin/ls", argv, environ);
		std::exit(EXIT_FAILURE);
	}
	return true;
}

int CGIResponseCreator::getMonitoredfd() const { return monitoredfd; }

void CGIResponseCreator::setMonitoredfd(CGIResponseCreator::tag const &_phase) {
	switch (_phase) {
		case CGIResponseCreator::CGIWRITE:
			monitoredfd = inpfd[1];
			break;
		case CGIResponseCreator::CGIRECV:
			monitoredfd = outpfd[0];
			break;
		default:
			break;
	}
}

bool CGIResponseCreator::writeMessageBody() const {
	// for develope
	if ((revents & POLLOUT) != POLLOUT) {
		return false;
	}
	write(inpfd[1], "Makefile", 8);
	return true;
}

bool CGIResponseCreator::recvCGIOutput() {
	ssize_t readLen;
	char buf[BUFFER_SIZE + 1] = {0};

	if ((revents & POLLIN) != POLLIN) {
		return false;
	}
	readLen = read(outpfd[0], buf, BUFFER_SIZE);
	if (readLen == -1) {
		// errorhandling
		putSytemError("read");
		return false;
	}
	if (readLen == 0) {
		phase = CGIResponseCreator::CGIRECVFIN;
		return true;
	}
	// for develope
	cgiOutput.append(buf, readLen);
	phase = CGIResponseCreator::CGIRECVFIN;
	return true;
}

bool CGIResponseCreator::setCGIOutput() {
	response.setBody(cgiOutput);
	return true;
}

bool CGIResponseCreator::deinit() {
	if (close(inpfd[0]) == -1) {
		putSytemError("read");
		// error handling
	}
	if (close(inpfd[1]) == -1) {
		// error handling
		putSytemError("read");
	}
	if (close(outpfd[0]) == -1) {
		// error handling
		putSytemError("read");
	}
	if (close(outpfd[1]) == -1) {
		// error handling
		putSytemError("read");
	}
	return true;
}
