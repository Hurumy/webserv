
#include "webserv.hpp"
#include "payload_classes/IPayload.hpp"
#include "payload_classes/Response.hpp"
#include "payload_classes/Request.hpp"

//request_parser
bool	parseRequest(Request &req, std::string rawData);

#define	FILE_READ_SIZE	500

static size_t	GetFilesize(std::string Filename)
{
	size_t 	bytes = 0;
	int		status = 0;
	int		fd;
	char	buf[FILE_READ_SIZE];

	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1)
		return (0);
	do
	{
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0)
			bytes += status;
	}
	while (status > 0);
	if (status == -1)
		return (0);
	close(fd);
	return (bytes);
}

static std::string	GetFileContents(std::string Filename)
{
	int				status;
	int				fd;
	char			buf[FILE_READ_SIZE];
	std::string		result;

	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1)
		return ("");
	do
	{
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0)
			result += buf;	
	}
	while (status > 0);

	if (status == -1)
		return ("");
	close(fd);
	return (result);
}

bool ReadFile(Response &response, std::string Filename)
{
	size_t		bytes;
	std::string	body;

	bytes = GetFilesize(Filename);
	if (bytes == 0)
		return (false);

	//バイト数をResponseにセットする
	response.setcontentLength(bytes);

	//中身読み込む
	body = GetFileContents(Filename);	
	if (body == "")
		return (false);

	//responseにセットする
	response.setbody(body);
	response.setcontentType("Content-Type: text/html;charset=UTF-8");

	return (true);
}

bool MakeResponse(Response &response)
{
	response.setversion("HTTP/1.1");
	response.setstatus(200);
	response.setstatusMessage("OK");
	response.setbody("Hello World!");
	response.setcontentType("Content-Type: text/plain");
	response.setcontentLength(12);

	return (true);
}



static std::string	ReadRequest(int fd)
{
	int				status;
	int				is_end = 0;
	char			buf[FILE_READ_SIZE];
	std::string		result;
	struct pollfd	s_poll;

	s_poll.fd = fd;
	s_poll.events = POLLIN;
	do
	{
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0)
			result += buf;
		if (poll(&s_poll, 1, 500) >= 0 && (s_poll.revents & POLLIN) == 0)
			is_end = 1;
	}
	while (status > 0 && is_end != 1);

	if (status == -1 || is_end != 1)
		return ("");
	return (result);
}

bool	MakeRequest(Request &request, int clientfd)
{
	bool		status;
	std::string req_rawdata;
	
	req_rawdata = ReadRequest(clientfd);
	status = parseRequest(request, req_rawdata);	
	if (status == false)
	{
		std::cout << "parseRequest failed" << std::endl;
		return (false);
	}
	//put out for debug
	std::cout << "==rawdata====" << std::endl;
	std::cout << req_rawdata << std::endl;
	std::cout << "=============" << std::endl;
	std::cout << "HEAD: "<< request.getmethod() << std::endl;
	std::cout << "URL: " << request.geturl() << std::endl;
	std::cout << "VERSION: " << request.getversion() << std::endl;
	std::cout << "Host: " << request.getmetadata("Host") << std::endl;
	std::cout << "User-Agent: " << request.getmetadata("User-Agent") << std::endl;
	std::cout << "Accept: " << request.getmetadata("Accept") << std::endl;
	std::cout << "Accept-Language: " << request.getmetadata("Accept-Language") << std::endl;
	std::cout << "Connection: " << request.getmetadata("Connection") << std::endl;
	std::cout << "Referer: " << request.getmetadata("Referer") << std::endl;
	std::cout << "Sec-Fetch-Dest: " << request.getmetadata("Sec-Fetch-Dest") << std::endl;
	std::cout << "Sec-Fetch-Mode: " << request.getmetadata("Sec-Fetch-Mode") << std::endl;		
	std::cout << "Sec-Fetch-Site: " << request.getmetadata("Sec-Fetch-Site") << std::endl;
	return (true);
}

int	SendResponse(Response &response, Request &request, int clientfd)
{
	std::string	line;
	int			status;

	//HTTPバージョンのチェック
	if (request.getversion().compare("HTTP/1.1\n") != 0)
	{
		std::cout << "SendResponse: Invalid Version(not HTTP/1.1)" << std::endl;
		return (false);
	}

	//responseに中身詰めます
	status = MakeResponse(response);		
	if (status == false)
	{
		std::cout << "Make Response is failed;;" << std::endl;
		return (false);
	}
	status = ReadFile(response, "content/readme.html");
	if (status == false)
	{
		std::cout << "HTML File Reading failed;;" << std::endl;
		return (false);
	}
	//レスポンスをWriteで書き込んで送信
	line = response.getLines();
	status = write(clientfd, line.c_str(), line.length());
	if (status == -1)
	{
		std::cout << "write syscall is failed;;" << std::endl;
		return (false);
	}	
	return (true);
}

int	MakeSocket(int &socketfd, sockaddr_in &s_bind)
{
	socklen_t	s_bind_siz;
	int			status;

	s_bind.sin_family = AF_INET;
	s_bind.sin_port = htons(8080);
	s_bind.sin_addr.s_addr = INADDR_ANY;
	s_bind_siz = sizeof(s_bind);

	//ソケット作るよ
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0)
	{
		std::cout << "socket syscall is failed;;" << std::endl;
		return (false);
	}

	//bindするよ
	status = bind(socketfd, (const struct sockaddr *)&s_bind, s_bind_siz);
	if (status == -1)
	{
		std::cout << "bind syscall is failed;;" << std::endl;
		return (false);
	}
	return (true);
}

int main()
{
	int			socketfd;
	sockaddr_in	s_bind;	
	socklen_t	s_bind_siz;
	int			clientfd;
	int			status;
	Request		*request;
	Response	*response;

	//ソケットを作る
	s_bind_siz = sizeof(s_bind);
	status = MakeSocket(socketfd, s_bind);
	if (status == false)
		std::cout << "MakeSocket failed;;" << std::endl;
	std::cout << "===Start Listening from the socket!" << std::endl;
	
	//Listen開始します
	listen(socketfd, 2);
	
	//ループで回します
	while (1)
	{
		//Accept待ちます
		std::cout << "Waiting for new connection by accept" << std::endl;
		clientfd = accept(socketfd, (struct sockaddr *)&s_bind, &s_bind_siz);
		
		//Acceptが通ったのでリクエストをReadします
		request = new Request();
		std::cout << "connected. then read" << std::endl << "========" << std::endl << std::endl;
		status = MakeRequest(*request, clientfd);
		if (status == false)
			std::cout << "MakeRequest Failed;;" << std::endl;

		//クライアントにおくるレスポンスを作る
		response = new Response();
		status = SendResponse(*response, *request, clientfd);
		if (status == false)
			std::cout << "SendResponse failed;;" << std::endl;
		
		//FDを閉じて次の接続をまつ
		close(clientfd);
		//delete response;
		//delete request;
		std::cout << std::endl << "========" << std::endl << "close the connection." << std::endl << std::endl;
	}
	
	return 0;
}
 
