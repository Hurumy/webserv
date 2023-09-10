/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler_test.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:14:49 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/10 19:11:35 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "SocketHandler.hpp"

TEST(SocketHandlerTest, constructorTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000);
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	ASSERT_EQ(ssockets.at(0).getPort(), 8080);
	ASSERT_EQ(ssockets.at(1).getPort(), 8000);
	ASSERT_EQ(ssockets.at(2).getPort(), 4040);
	socketHandler.addCSocket(CSocket(0));
	std::vector<CSocket> const &csockets = socketHandler.getCSockets();
	ASSERT_EQ(csockets.at(0).getRevents(), 0);
}

TEST(SocketHandlerTest, pollfdsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.createPollfds();
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	std::vector<struct pollfd> const &pollfds = socketHandler.getPollfds();
	ASSERT_EQ(pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(pollfds.at(0).events, POLLIN | POLLOUT | POLLHUP);
}

TEST(SocketHandlerTest, setReventsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 100));
	sources.push_back(SSocket(8000, IPV4, 100));
	sources.push_back(SSocket(4040, IPV4, 100));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.createPollfds();
	std::vector<SSocket> const &ssockets = socketHandler.getSSockets();
	std::vector<struct pollfd> const &pollfds = socketHandler.getPollfds();
	ASSERT_EQ(pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(pollfds.at(0).events, POLLIN | POLLOUT | POLLHUP);
	ASSERT_TRUE(socketHandler.setRevents());
	std::vector<struct pollfd> const &resutl_pollfds = socketHandler.getPollfds();
	ASSERT_EQ(resutl_pollfds.at(0).fd, ssockets.at(0).getSockfd());
	ASSERT_EQ(resutl_pollfds.at(0).revents, ssockets.at(0).getRevents());
	ASSERT_EQ(resutl_pollfds.at(1).revents, ssockets.at(1).getRevents());
}

TEST(SocketHandlerTest, recieveCSocketsTest) {
	std::vector<SSocket> sources;

	sources.push_back(SSocket(8080, IPV4, 1));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	pid_t pid = fork();
	if (pid == -1) {
		std::exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		sleep(1);
		system("curl localhost:8080");
		std::exit(EXIT_SUCCESS);
	}
	while (true) {
		socketHandler.recieveCSockets();
		if (socketHandler.getCSockets().size() != 0) {
			break ;
		}
		socketHandler.clearPollfds();
		socketHandler.createPollfds();
		socketHandler.setRevents();
	}
	socketHandler.closeAllSSockets();
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int main_simple_http_client() {
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    // char buffer[1024] = {0};

    const int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sock) {
        perror("simple HTTP client: sock");
        return EXIT_FAILURE;
    }
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    if (0 >= inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)) {
        perror("simple HTTP client: inet_pton");
        return EXIT_FAILURE;
    }
    if (0 > connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
        perror("simple HTTP client: connect");
        return EXIT_FAILURE;
    }
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    return 0;
}

TEST(SocketHandlerTest, getDataMapTest) {
	std::vector<SSocket> sources;
	std::map<int, std::string> request;

	sources.push_back(SSocket(8081, IPV4, 1));
	SocketHandler socketHandler(sources, 1000);
	socketHandler.initAllSSockets();
	socketHandler.createPollfds();
	socketHandler.setRevents();
	pid_t pid = fork();
	if (pid == -1) {
		std::exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		sleep(1);
		main_simple_http_client();
		std::exit(EXIT_SUCCESS);
	}
	while (true) {
		socketHandler.recieveCSockets();
		if (socketHandler.getCSockets().empty() == false) {
			Result<std::map<int, std::string>, bool> dataMap = socketHandler.getDataMap();
			if (dataMap.isOK() == true) {
				request = dataMap.getOk();
				if (request.empty() == false) {
					break ;
				}
			}
		}
		socketHandler.clearPollfds();
		socketHandler.removeClosedCSockets();
		socketHandler.createPollfds();
		socketHandler.setRevents();
	}
	socketHandler.closeAllSSockets();
	ASSERT_STREQ(request.at(5).c_str(), "Hello from client");
}
