
#pragma once

#include "webserv.hpp"

class IPayload
{
	private:
	protected:
		std::string 						version;
		std::string							body;
		std::map<std::string, std::string> 	metadata;
	public:
		virtual const std::string	getLines() const = 0;
		const bool					setversion(std::string version);
		const bool					setbody(std::string body);
};

