
#include "Response.hpp"

const std::string	Response::getLines(void) const
{
	std::string line;

	line += this->version;
	line += this->statusMessage;
	line += this->body;

	return (line);
}

const bool	Response::setstatus(unsigned int status)
{
	this->status = status;
	return (true);
}

const bool Response::setstatusMessage(std::string message)
{
	this->statusMessage = message;
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

