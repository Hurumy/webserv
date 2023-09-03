
#include "Response.hpp"

#include <sstream>

const std::string	Response::getLines() const
{
	std::string line;
	std::ostringstream numStatus;
	std::ostringstream numContentLength;

	line += version;
	line += " ";
	numStatus << status;
	line += numStatus.str();
	line += " ";
	line += statusMessage;
	line += "\r\n";
	for (std::map<std::string, std::string>::const_iterator iter = header.begin(); iter != header.end(); ++iter) {
		line += iter->first;
		line += ": ";
		line += iter->second;
		line += "\r\n";
	}
	line += "\r\n";
	line += body;
	return (line);
}

bool Response::setStatus(unsigned int _status)
{
	status = _status;
	return (true);
}

bool Response::setStatusMessage(std::string const &message)
{
	statusMessage = message;
	return (true);
}

unsigned int Response::getStatus() const
{
	return(status);
}

std::string const &Response::getStatusMessage() const
{
	return(statusMessage);
}
