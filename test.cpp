
#include "webserv.hpp"
#include "IPayload.hpp"
#include "Response.hpp"
#include "Request.hpp"

int main()
{
	int			socketfd;
	sockaddr_in	s_bind;	
	socklen_t	s_bind_siz;
	char		buf[1000];
	int			clientfd;

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
		
		response->setversion("HTTP/1.1");
		response->setstatus(200);
		response->setstatusMessage("OK");
		response->setbody("Hello World!");
		response->setcontentType("Content-Type: text/plain");
		response->setcontentLength(12);

		line = response->getLines();
		write(clientfd, line.c_str(), line.length());
		
		//FDを閉じて次の接続をまつ
		close(clientfd);
		delete response;
	}
	
	return 0;
}







