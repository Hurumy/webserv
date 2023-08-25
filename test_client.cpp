
#include "webserv.hpp"

int main()
{
	int socketfd;
	sockaddr_in serv_addr;
	char message[] = "Heelloo from client";
	char buf[1000];	

	socketfd = socket(PF_INET, SOCK_STREAM, 0);
}


