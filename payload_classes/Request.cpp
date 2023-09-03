
#include "Request.hpp"

const std::string	Request::getLines() const
{
	std::string	line;

	line += this->version;
	line += this->body;
	line += this->url;
	return (line);
}

bool	Request::seturl(std::string url)
{
	this->url = url;
	return (true);
}

std::string const	Request::geturl(void) const
{
	return (this->url);
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
	std::cout << test->geturl() << std::endl;
	std::cout << test->getversion() << std::endl;
	std::cout << test->getbody() << std::endl;

	delete test;
	return (0);
}*/


