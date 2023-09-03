
#include "webserv.hpp"
#include "IPayload.hpp"
#include "Response.hpp"
#include "Request.hpp"

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

int main()
{
	int			socketfd;
	sockaddr_in	s_bind;	
	socklen_t	s_bind_siz;
	int			clientfd;
	int			status;
	std::string	request;

	s_bind.sin_family = AF_INET;
	s_bind.sin_port = htons(8080);
	s_bind.sin_addr.s_addr = INADDR_ANY;
	s_bind_siz = sizeof(s_bind);

	//ソケット作るよ
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0)
		return (-1);

	//bindするよ
	bind(socketfd, (const struct sockaddr *)&s_bind, s_bind_siz);
	std::cout << "Start Listening from the socket!" << std::endl;
	
	//Listen開始します
	listen(socketfd, 2);
	
	//ループで回します
	while (1)
	{
		//Accept待ちます
		std::cout << "Waiting for new connection by accept" << std::endl;
		clientfd = accept(socketfd, (struct sockaddr *)&s_bind, &s_bind_siz);
		
		//Acceptが通ったのでリクエストをReadします
		std::cout << "connected. then read" << std::endl << "========" << std::endl << std::endl;
		request = ReadRequest(clientfd);
		std::cout << "Request: " << request << std::endl;

		//クライアントにおくるレスポンスを作る
		Response *response = new Response();
		std::string	line;

		//responseに中身詰めます
		MakeResponse(*response);		
		status = ReadFile(*response, "readme.html");
		if (status == false)
			std::cout << "reading failed;;" << std::endl;

		//レスポンスをWriteで書き込んで送信
		line = response->getLines();
		write(clientfd, line.c_str(), line.length());
		
		//FDを閉じて次の接続をまつ
		close(clientfd);
		std::cout << std::endl << "========" << std::endl << "close the connection." << std::endl << std::endl;
	}
	
	return 0;
}
 
