
#include "../webserv.hpp"
#include "../payload_classes/Request.hpp"

/*
bool	parseRequest(Request &req, std::string rawData)
{
		
}*/


int main()
{
	Request test;

	test.addmetadata("test", "value");
	std::cout << test.getmetadata("test") << std::endl;
	return (0);
}

