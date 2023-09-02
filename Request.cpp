
#include "Request.hpp"

const std::string	Request::getLines() const
{
	std::string	line;

	line += this->version;
	line += this->body;
	line += this->url;
	return (line);
}

const bool	Request::seturl(std::string url)
{
	this->url = url;
	return (true);
}

//test code
/*
int main()
{
	Request *test = new Request();

	test->setversion("v1.2\n");
	test->seturl("http:\n");
	test->setbody("NICE BODY~\n");
	
	std::cout << test->getLines() << std::endl;

	delete test;
	return (0);
}
*/

