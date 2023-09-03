
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

std::string const	IPayload::getversion(void) const
{
	return(this->version);
}

std::string const	IPayload::getbody(void) const
{
	return (this->body);
}

bool	IPayload::setmethod(std::string method) 
{
	this->method = method;
	return (true);
}

std::string const	IPayload::getmethod(void) const
{
	return (this->method);
}

