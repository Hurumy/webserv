
#include "Response.hpp"

const std::string	Response::getLines(void) const
{
	std::string line;

	line += this->version;
	line += this->status;
	line += this->statusMessage;
	line += "\n";
	line += this->contentType;
	line += "\n";
	line += "Content-Length: ";
	line += this->contentLength;
	line += "\n";
	line += "\n";
	line += this->body;

	return (line);
}

bool	Response::setstatus(unsigned int status)
{
	this->status = status;
	return (true);
}

bool Response::setstatusMessage(std::string message)
{
	this->statusMessage = message;
	return (true);
}

bool	Response::setcontentType(std::string type)
{
	this->contentType = type;
	return (true);
}

bool	Response::setcontentLength(size_t len)
{
	this->contentLength = len;
	return (true);
}

//test code
/*
int main()
{
	Response	*test = new Response();

	test->setversion("version1.2.3\n");
	test->setbody("NICE BODY~\n");
	test->setstatusMessage("WTF!!\n");
	std::cout << test->getLines() << std::endl;

	delete test;
	return 0;
}
*/

