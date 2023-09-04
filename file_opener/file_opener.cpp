
#include "../webserv.hpp"
#include "../payload_classes/IPayload.hpp"
#include "../payload_classes/Response.hpp"
#include "../payload_classes/Request.hpp"

int	setcontentType(Request req, Response &res)
{
	std::string	file = req.geturl();
	size_t		len = req.geturl().size();
	std::string	checkstr;
	std::map<std::string, std::string>	ext;

	ext[]

	if (file == "")
		return (0);
	checkstr = file.substr(len - 5, len);
	if (checkstr == ".html")
		result = "text/html";
	checkstr
	res.setcontentType()
}

int	fileOpener(Request req, Response &res)
{
	int			status;
	int			fd;
	char		buf[FILE_READ_SIZE];
	std::string	body;

	if (req.geturl() == "")
		return (0);
	status = open(req.geturl(), O_RDONLY);
	if (status == -1)
		return (-1);
	fd = status;
	for (int i = 0; status > 0; i ++)
	{
		status = read(fd, buf, FILE_READ_SIZE);
		if (status != -1)
			body += buf;
	}
	status = res.setbody(body);
	if (status == false)
		return (-1);
	return (0);
}

