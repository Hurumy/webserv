/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:54:44 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/17 00:38:16 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIResponseCreator.hpp"

#include <cstring>

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

// リソースへの絶対パス
// CGI script のファイル名
bool CGIResponseCreator::_setPathInfo() const {
	// cgi スクリプトを示す path 以下の path (query string は含まない) をセット
	return true;
}

bool CGIResponseCreator::_setPathTranslated() const {
	// cgi スクリプトの実際のロケーション + PATH_INFO
	return true;
}

bool CGIResponseCreator::_setQuerySring() const {
	// Query string
	return true;
}

bool CGIResponseCreator::setEnvVars() {
	// For develope
	request.getPhase();
	_setPathInfo();
	_setPathTranslated();
	_setQuerySring();
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
