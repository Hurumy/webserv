
#pragma once

#include "../webserv.hpp"
#include "IPayload.hpp"

class Response: public IPayload
{
	private:
		unsigned int	status;
		std::string		statusMessage;
		std::string		contentType;
		size_t			contentLength;
	protected:
	public:
		std::string const	getLines(void) const;
		bool				setstatus(unsigned int status);
		unsigned int		getstatus(void) const;
		bool				setstatusMessage(std::string message);
		std::string const	getstatusMessage(void) const;
		bool				setcontentType(std::string type);
		std::string const	getcontentType(void) const;
		bool				setcontentLength(size_t len);
		size_t 				getcontentLength(void) const;
};


