/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:21 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/05 15:55:10 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

// request_parser
bool parseRequest(Request &req, std::string rawData);
int makeResBody(Request req, Response &res);

#define FILE_READ_SIZE 500

int makeSocket(int &socketfd, sockaddr_in &s_bind) {
	socklen_t s_bind_siz;
	int status;

	s_bind.sin_family = AF_INET;
	s_bind.sin_port = htons(DEFAULT_PORT);
	s_bind.sin_addr.s_addr = INADDR_ANY;
	s_bind_siz = sizeof(s_bind);

	//ソケット作るよ
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
		std::cout << RED "socket syscall is failed;;" RESET << std::endl;
		return (false);
	}

	// bindするよ
	status = bind(socketfd, (const struct sockaddr *)&s_bind, s_bind_siz);
	if (status == -1) {
		std::cout << RED "bind syscall is failed;;" RESET << std::endl;
		return (false);
	}
	return (true);
}

static std::string readRequest(int fd) {
	int 			status;
	int 			is_end = 0;
	char 			buf[FILE_READ_SIZE];
	std::string 	result;
	struct pollfd 	s_poll;

	s_poll.fd = fd;
	s_poll.events = POLLIN;
	do {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0) result += buf;
		if (poll(&s_poll, 1, 1000) >= 0 && (s_poll.revents & POLLIN) == 0)
			is_end = 1;
	} while (status > 0 && is_end != 1);
	if (status == -1 || is_end != 1) return ("");
	return (result);
}

bool makeRequest(Request &request, int clientfd) {
	bool status;
	std::string req_rawdata;

	req_rawdata = readRequest(clientfd);
	status = parseRequest(request, req_rawdata);
	if (status == false) {
		std::cout << RED "parseRequest failed" RESET << std::endl;
		return (false);
	}

	// put out for debug
	//std::cout << "==rawdata====" << std::endl;
	//std::cout << req_rawdata << std::endl;
	std::cout << CYAN "=PARSED REQUEST============" << std::endl;
	std::cout << "HEAD: " << request.getMethod() << std::endl;
	std::cout << "URL: " << request.getUrl() << std::endl;
	std::cout << "VERSION: " << request.getVersion() << std::endl;
	std::cout << "Host: " << request.getHeader("Host").getOk() << std::endl;
	std::cout << "User-Agent: " << request.getHeader("User-Agent").getOk()
			  << std::endl;
	std::cout << "Accept: " << request.getHeader("Accept").getOk() << std::endl;
	std::cout << "Accept-Language: "
			  << request.getHeader("Accept-Language").getOk() << std::endl;
	std::cout << "Connection: " << request.getHeader("Connection").getOk()
			  << std::endl;
	std::cout << "Referer: " << request.getHeader("Referer").getOk()
			  << std::endl;
	std::cout << "Sec-Fetch-Dest: "
			  << request.getHeader("Sec-Fetch-Dest").getOk() << std::endl;
	std::cout << "Sec-Fetch-Mode: "
			  << request.getHeader("Sec-Fetch-Mode").getOk() << std::endl;
	std::cout << "Sec-Fetch-Site: "
			  << request.getHeader("Sec-Fetch-Site").getOk() << std::endl;
	std::cout << "===========================" RESET << std::endl;
	return (true);
}

bool makeResponse(Request &request, Response &response) {
	int	status;
	
	response.setVersion("HTTP/1.1");
	status = makeResBody(request, response);
	if (status == -1)
		return (false);
	return (true);
}

int sendResponse(Response &response, Request &request, int clientfd) {
	std::string line;
	int status;

	response.setStatus(200);
	response.setStatusMessage("OK");
	
	// HTTPバージョンのチェック
	if (request.getVersion().compare("HTTP/1.1") != 0) {
		std::cout << RED "SendResponse: Invalid Version(not HTTP/1.1)" RESET << std::endl;
		response.setStatus(505);
		response.setStatusMessage("HTTP Version Not Supported");
	}

	// responseに中身詰めます
	status = makeResponse(request, response);
	if (status == false) {
		std::cout << RED "Make Response is failed;; STATUSCODE: " << response.getStatus() << RESET << std::endl;
	}
	std::cout << YELLOW "=Maked Response=======" << std::endl;
	std::cout << response.getLines() << std::endl;
	std::cout << "======================" RESET << std::endl;
	
	//レスポンスをWriteで書き込んで送信
	line = response.getLines();
	status = write(clientfd, line.c_str(), line.length());
	if (status == -1) {
		std::cout << RED "write syscall is failed;;" RESET << std::endl;
		return (false);
	}
	return (true);
}

int main() {
	int				socketfd;
	sockaddr_in		s_bind;
	socklen_t		s_bind_siz;
	int				clientfd;
	bool			status;
	Request			*request;
	Response		*response;
	//ソケットを作る
	s_bind_siz = sizeof(s_bind);
	status = makeSocket(socketfd, s_bind);
	if (status == false) std::cout << RED "makeSocket failed;;" RESET << std::endl;
	std::cout << GREY "===Start Listening from the socket!" RESET << std::endl;

	// Listen開始します
	listen(socketfd, 2);

	//ループで回します
	while (1) {
		// Accept待ちます
		std::cout << GREY "Waiting for new connection by accept" RESET << std::endl;
		clientfd = accept(socketfd, (struct sockaddr *)&s_bind, &s_bind_siz);

		// Acceptが通ったのでリクエストをReadします
		request = new Request();
		std::cout << GREY "connected. then read" << std::endl
				  << "========" RESET << std::endl
				  << std::endl;
		status = makeRequest(*request, clientfd);
		if (status == false) std::cout << RED "MakeRequest Failed;;" RESET << std::endl;

		//クライアントにおくるレスポンスを作る
		response = new Response();
		status = sendResponse(*response, *request, clientfd);
		if (status == false) std::cout << RED "SendResponse failed;;" RESET << std::endl;

		// FDを閉じて次の接続をまつ
		close(clientfd);
		// delete response;
		// delete request;
		std::cout << std::endl
				  << GREY "========" << std::endl
				  << "close the connection." RESET << std::endl
				  << std::endl;
	}

	return 0;
}

