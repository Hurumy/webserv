
#include "IPayload.hpp"

bool	IPayload::setversion(std::string version)
{
	this->version = version;
	return (true);
}

bool	IPayload::setbody(std::string body)
{
	this->body = body;
	return (true);
}

std::string const	IPayload::getmetadata(std::string key) const
{	
	if (this->metadata.find(key) == this->metadata.end())
		return ("");
	else
		return (this->metadata.at(key));
}

bool	IPayload::addmetadata(std::string key, std::string value)
{
	if (this->metadata.find(key) != this->metadata.end())
		return (false);
	else
		this->metadata[key] = value;
	return (true);
}

