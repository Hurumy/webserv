
#pragma once

#include "../webserv.hpp"

class IPayload
{
	private:
	protected:
		std::string 						version;
		std::string							body;
		std::map<std::string, std::string> 	metadata;
	public:
		virtual const std::string	getLines() const = 0;
		bool						setversion(std::string version);
		bool						setbody(std::string body);
		std::string const			getmetadata(std::string key) const;
		bool						addmetadata(std::string key, std::string value);
};

