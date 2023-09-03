
#pragma once

#include "../webserv.hpp"
#include "IPayload.hpp"

class Request: public IPayload
{
	private:
		std::string	url;
		std::string method;
	protected:
	public:
		const std::string	getLines() const;
		bool				seturl(std::string url);
		std::string const	geturl(void) const;
		bool				setmethod(std::string method);
		std::string const	getmethod(void) const;
};


