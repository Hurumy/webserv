/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:36:35 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/17 23:49:57 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "MetaVariables.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define WS_HTTP_VERSION "HTTP/1.1"

class CGIResponseCreator {
	public:
		enum tag { CGISTARTUP, CGIWRITE, CGIRECV, CGIRECVFIN };
		CGIResponseCreator(Request &_request, Response &_response, const std::string &_cgiPath);

		CGIResponseCreator::tag const &getPhase() const;
		void setPhase(CGIResponseCreator::tag const &_phase);
		short getRevents() const;
		void setRevents(short const _revents);
		bool setEnvVars();
		bool execCGIScript();
		int getMonitoredfd() const;
		void setMonitoredfd(CGIResponseCreator::tag const &_phase);
		bool writeMessageBody() const;
		bool recvCGIOutput();
		bool setCGIOutput();
		bool deinit();

	protected:
	private:
		bool _setPathInfo();
		bool _setPathTranslated();
		bool _setQuerySring();
		bool _setRemoteAddr();
		bool _setRequestMethod();
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
		MetaVariables metaVariables;
};
