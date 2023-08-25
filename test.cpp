
#include "webserv.hpp"

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

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0)
		return (-1);
	bind(socketfd, (const struct sockaddr *)&s_bind, s_bind_siz);
	std::cout << "Start Listening from the socket!" << std::endl;
	listen(socketfd, 2);
	while (1)
	{
		std::cout << "Waiting for new connection by accept" << std::endl;
		clientfd = accept(socketfd, (struct sockaddr *)&s_bind, &s_bind_siz);
		std::cout << "connected. then read" << std::endl;
		read(clientfd, buf, 1000);
		printf("%s\n", buf);
		printf("return ok message\n");
		write(clientfd, "listen ok!", 11);
		close(clientfd);
	}
	return 0;
}







