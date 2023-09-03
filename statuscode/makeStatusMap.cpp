
#include "../webserv.hpp"

std::map<int, std::string>	makeStatusMap()
{
	std::map<int, std::string>	result;
	result[100] = "Continue";
	result[101] = "Switching Protocol";
	result[102] = "Processing";
	result[103] = "Early Hints";
	result[200] = "OK";
	result[201] = "Created";
	result[202] = "Accepted";
	result[203] = "Non-Authoritative Information";
	result[204] = "No Content";
	result[205] = "Reset Content";
	result[206] = "Partial Content";
	result[300] = "Multiple Choice";
	result[301] = "Moved Permanently";
	result[302] = "Found";
	result[304] = "Not Modified";
	result[400] = "Bad Request";
	result[401] = "Unauthorized";
	result[402] = "Payment Required";
	result[403] = "Forbidden";
	result[404] = "Not Found";
	result[405] = "Method Not Allowed";
	result[406] = "Not Acceptable";
	result[407] = "Proxy Authentication Required";
	result[408] = "Request Timeout";
	result[409] = "Conflict";
	result[410] = "Gone";
	result[411] = "Length Required";
	result[412] = "Precondition Failed";
	result[413] = "Payload Too Large";
	result[414] = "URI Too Long";
	result[415] = "Unsupported Media Type";
	result[416] = "Range Not Satisfiable";
	result[417] = "Expectation Failed";
	result[422] = "Unprocessable Entity";
	result[423] = "Locked";
	result[425] = "Too Early";
	result[426] = "Upgrade Required";
	result[429] = "Too Many Requests";
	result[431] = "Request Header Fields Too Large";
	result[500] = "Internal Server Error";
	result[501] = "Not Implemented";
	result[502] = "Bad Gateway";
	result[503] = "Service Unavailable";
	result[504] = "Gateway Timeout";
	result[505] = "HTTP Version Not Supported";
	result[506] = "Variant Also Negotiates";
	result[507] = "Insufficient Storage";
	result[508] = "Loop Detected";
	result[510] = "Not Extended";
	result[511] = "Network Authentication Required";
	return (result);
}

/*
int main(void)
{
	std::map<int, std::string>	test;

	test = makeStatusMap();
	std::cout << test.at(800) << std::endl;
	return (0);
}*/


