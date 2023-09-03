
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
	for (std::map<std::string, std::string>::const_iterator iter = metadata.begin(); iter != metadata.end(); ++iter) {
		line += iter->first;
		line += ": ";
		line += iter->second;
		line += "\r\n";
	}
	line += "Content-Type: ";
	line += contentType;
	line += "\r\n";
	line += "Content-Length: ";
	numContentLength << contentLength;
	line += numContentLength.str();
	line += "\r\n";
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

bool Response::setContentType(std::string const &_type)
{
	contentType = _type;
	return (true);
}

bool Response::setContentLength(size_t const len)
{
	contentLength = len;
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

std::string const &Response::getContentType() const
{
	return(contentType);
}

size_t Response::getContentLength() const
{
	return (contentLength);
}
