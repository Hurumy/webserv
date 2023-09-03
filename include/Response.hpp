/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:28:25 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/03 15:43:38 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "APayload.hpp"


class Response: public virtual APayload
{
	private:
		unsigned int		status;
		std::string			statusMessage;
		std::string			contentType;
		size_t				contentLength;
	protected:
	public:
		std::string const	getLines() const;
		bool				setStatus(unsigned int status);
		unsigned int		getStatus() const;
		bool				setStatusMessage(std::string const &message);
		std::string const	&getStatusMessage() const;
		bool				setContentType(std::string const &type);
		std::string const	&getContentType() const;
		bool				setContentLength(size_t const len);
		size_t				getContentLength() const;
};
