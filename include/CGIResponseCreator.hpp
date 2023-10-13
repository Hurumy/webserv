/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIResponseCreator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:36:35 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/13 13:06:34 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "Response.hpp"

class CGIResponseCreator {
	public:
		enum tag {
			CGISTARTUP,
			CGIWRITE,
			CGIRECV,
			CGIRECVFIN
		};
		CGIResponseCreator(Request &_request, Response &_response);

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
		Request &request;
		Response &response;
		CGIResponseCreator::tag phase;
		int inpfd[2];
		int outpfd[2];
		int monitoredfd;
		short revents;
		std::string cgiOutput;
};
