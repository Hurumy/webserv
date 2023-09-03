
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
		bool				setstatusMessage(std::string message);
		bool				setcontentType(std::string type);
		bool				setcontentLength(size_t len);
};


