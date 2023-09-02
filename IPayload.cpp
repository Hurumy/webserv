
#include "IPayload.hpp"

const bool	IPayload::setversion(std::string version)
{
	this->version = version;
	return (true);
}

const bool	IPayload::setbody(std::string body)
{
	this->body = body;
	return (true);
}

