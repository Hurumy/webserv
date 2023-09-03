
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

