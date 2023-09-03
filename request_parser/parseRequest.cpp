
#include "../webserv.hpp"
#include "../payload_classes/Request.hpp"

static std::vector<std::string> lineSpliter(std::string origin, std::string delim)
{
	std::vector<std::string>	list;
	int							pos;
	int							delimlen = delim.length();
	int							offset;

	if (delimlen == 0)
		list.push_back(origin);
	else
	{
		offset = 0;
		pos = 0;
		while (1)
		{
			pos = origin.find(delim, offset);
			if (pos == std::string::npos)
			{
				list.push_back(origin.substr(offset));
				break;
			}
			list.push_back(origin.substr(offset, pos - offset));
			offset = pos + delimlen;
		}
	}
	return (list);
}

/*
static bool	parseFirstLine(Request &req, std::string Firstline)
{
	std::vector<std::string> elems;

	elems = lineSpliter(Firstline, " ");
	for (int i = 0; i < elems.size(); i ++)
		std::cout << elems.at(i) << std::endl;
	
	req.set
}


bool	parseRequest(Request &req, std::string rawData)
{
	std::vector<std::string> lines;
	int	status;

	lines = lineSpliter(rawData, "\n");
	
	status = parseFirstLine(req, lines.at(0));
	if (status == false)
		return (false);
	
}
*/

/*
int main()
{
	Request test;
	std::vector<std::string> lines;

	test.addmetadata("test", "value");
	std::cout << test.getmetadata("test") << std::endl;
	lines = lineSpliter("test1, test2, test3, what is this?", ", ");
	for (int i = 0; i < lines.size(); i ++)
		std::cout << lines.at(i) << std::endl;
	return (0);
}*/

