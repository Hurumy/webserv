
#include "webserv.hpp"
#include "IPayload.hpp"
#include "Response.hpp"
#include "Request.hpp"

bool ReadFile(Response &response, std::string Filename)
{
	int			bytes = 0;
	int			status;
	int			fd;
	char		buf[500];
	std::string	body;

	//バイト数を調べる
	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1)
		return (false);
	do
	{
		status = read(fd, buf, 500);
		if (status > 0)
			bytes += status;
	}
	while (status > 0);
	if (bytes == -1)
		return (false);
	close(fd);

	//バイト数をResponseにセットする
	response.setcontentLength(bytes);

	bytes = 0;
	//中身読み込む
	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1)
		return (false);
	do
	{
		status = read(fd, buf, 500);
		if (status > 0)
		{
			bytes += status;
			body += buf;
		}
	}
	while (status > 0);
	

	if (bytes == -1)
		return (false);
	close(fd);

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

int main()
{
	int			socketfd;
	sockaddr_in	s_bind;	
	socklen_t	s_bind_siz;
	char		buf[1000];
	int			clientfd;
	int			status;

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
		
		//Acceptが通ったのでReadします(たくさん)
		std::cout << "connected. then read" << std::endl;
		read(clientfd, buf, 1000);
		printf("%s\n", buf);
		printf("--That is all message. Start Sending.\n");

		//クライアントにデータを送信します
		Response *response = new Response();
		std::string	line;

		//responseに中身詰めます
		MakeResponse(*response);		
		status = ReadFile(*response, "readme.html");
		if (status == false)
			std::cout << "reading failed;;" << std::endl;

		line = response->getLines();
		write(clientfd, line.c_str(), line.length());
		
		//FDを閉じて次の接続をまつ
		close(clientfd);
	}
	
	return 0;
}


