
#pragma once

#include "../webserv.hpp"
#include "IPayload.hpp"

class Request: public IPayload
{
	private:
		std::string	url;
	protected:
	public:
		const std::string	getLines() const;
		bool				seturl(std::string url);
		std::string const	geturl(void) const;
};


