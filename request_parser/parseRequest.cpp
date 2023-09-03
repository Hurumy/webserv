
#include "../webserv.hpp"
#include "../payload_classes/Request.hpp"


//単純なSplit Delimは完全一致 必要ならStatic外して使ってください
static std::vector<std::string> lineSpliter(std::string origin, std::string delim)
{
	std::vector<std::string>	list;
	long						pos;
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
			if ((unsigned long)pos == std::string::npos)
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

//1行目をパースしてRequestに中身をセットする
static bool	parseFirstLine(Request &req, std::string Firstline)
{
	std::vector<std::string> elems;

	elems = lineSpliter(Firstline, " ");
	if (elems.size() >= 3)
	{
		req.setmethod(elems.at(0));
		req.seturl(elems.at(1));
		req.setversion(elems.at(2));
	}
	else
		return (false);
	return (true);
}

//ヘッダーのうち1行をもらい、RequestにMetadataとして詰める
static bool	addParsedLine(Request &req, std::string line)
{
	std::vector<std::string> 	tmp;
	bool						status;

	tmp = lineSpliter(line, ": ");
	if (tmp.size() != 2)
		return (false);
	status = req.addmetadata(tmp.at(0), tmp.at(1));
	if (status == false)
		return (false);
	return (true);
}

//リクエストをパースして参照に詰める
bool	parseRequest(Request &req, std::string rawData)
{
	std::vector<std::string> lines;
	bool					status;
	
	lines = lineSpliter(rawData, "\n");
	status = parseFirstLine(req, lines.at(0));
	if (status == false)
		return (false);
	for (unsigned long i = 1; i < lines.size() - 2; i ++)
	{
		status = addParsedLine(req, lines.at(i));
		if (status == false)
		{
			std::cout << "lines.size: " << lines.size() << std::endl;
			std::cout << "parse failed line: " << lines.at(i) << std::endl;
			std::cout << "parse failed line: " << i << std::endl;
			return (false);
		}
	}
	return (true);
}

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
	std::cout << "====" << std::endl;

	parseRequest(test, "GET / HTTP/1.1\t\nHost: xxx\nUser-Agent: FireFox\nAccept: ???\n\n");
	
	std::cout << test.getmethod() << std::endl;
	std::cout << test.geturl() << std::endl;
	std::cout << test.getversion() << std::endl;

	std::cout << test.getmetadata("Host") << std::endl;
	std::cout << test.getmetadata("User-Agent") << std::endl;
	std::cout << test.getmetadata("Accept") << std::endl;

	return (0);
}
*/

