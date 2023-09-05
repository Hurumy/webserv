/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:21 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/05 11:06:09 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "APayload.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

// request_parser
bool parseRequest(Request &req, std::string rawData);

#define FILE_READ_SIZE 500

// std::string	ft_stoull(unsigned long long num)
// {
// 	std::string	res;
// }

static size_t GetFilesize(std::string Filename) {
	size_t bytes = 0;
	int status = 0;
	int fd;
	char buf[FILE_READ_SIZE];

	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1) return (0);
	do {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0) bytes += status;
	} while (status > 0);
	if (status == -1) return (0);
	close(fd);
	return (bytes);
}

static std::string GetFileContents(std::string Filename) {
	int status;
	int fd;
	char buf[FILE_READ_SIZE];
	std::string result;

	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1) return ("");
	do {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0) result += buf;
	} while (status > 0);
	if (status == -1) return ("");
	close(fd);
	return (result);
}

bool ReadFile(Response &response, std::string Filename) {
	unsigned long long bytes;
	std::string body;

	bytes = GetFilesize(Filename);
	if (bytes == 0) return (false);

	//バイト数をResponseにセットする
	response.addHeader("Content-Length", "7000");

	//中身読み込む
	body = GetFileContents(Filename);
	if (body == "") return (false);

	// responseにセットする
	response.setBody(body);
	response.addHeader("Content-Type", "text/html;charset=UTF-8");

	return (true);
}

bool MakeResponse(Response &response) {
	response.setVersion("HTTP/1.1");
	response.setStatus(200);
	response.setStatusMessage("OK");
	response.setBody("Hello World!");
	// response.addHeader("Content-Type", "text/plain");
	// response.addHeader("Content-Length", "12");

	return (true);
}

static std::string ReadRequest(int fd) {
	int status;
	int is_end = 0;
	char buf[FILE_READ_SIZE];
	std::string result;
	struct pollfd s_poll;

	s_poll.fd = fd;
	s_poll.events = POLLIN;
	do {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0) result += buf;
		if (poll(&s_poll, 1, 500) >= 0 && (s_poll.revents & POLLIN) == 0)
			is_end = 1;
	} while (status > 0 && is_end != 1);

	if (status == -1 || is_end != 1) return ("");
	return (result);
}

bool MakeRequest(Request &request, int clientfd) {
	bool status;
	std::string req_rawdata;

	req_rawdata = ReadRequest(clientfd);
	status = parseRequest(request, req_rawdata);
	if (status == false) {
		std::cout << "parseRequest failed" << std::endl;
		return (false);
	}

	// put out for debug
	std::cout << "==rawdata====" << std::endl;
	std::cout << req_rawdata << std::endl;
	std::cout << "=============" << std::endl;
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
	return (true);
}

int SendResponse(Response &response, Request &request, int clientfd) {
	std::string line;
	int status;

	// HTTPバージョンのチェック
	if (request.getVersion().compare("HTTP/1.1\r\n") != 0) {
		std::cout << "SendResponse: Invalid Version(not HTTP/1.1)" << std::endl;
		// return (false);
	}

	// responseに中身詰めます
	status = MakeResponse(response);
	if (status == false) {
		std::cout << "Make Response is failed;;" << std::endl;
		return (false);
	}
	status = ReadFile(response, "content/readme.html");
	if (status == false) {
		std::cout << "HTML File Reading failed;;" << std::endl;
		return (false);
	}
	//レスポンスをWriteで書き込んで送信
	line = response.getLines();
	status = write(clientfd, line.c_str(), line.length());
	if (status == -1) {
		std::cout << "write syscall is failed;;" << std::endl;
		return (false);
	}
	return (true);
}

int MakeSocket(int &socketfd, sockaddr_in &s_bind) {
	socklen_t s_bind_siz;
	int status;

	s_bind.sin_family = AF_INET;
	s_bind.sin_port = htons(8080);
	s_bind.sin_addr.s_addr = INADDR_ANY;
	s_bind_siz = sizeof(s_bind);

	//ソケット作るよ
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
		std::cout << "socket syscall is failed;;" << std::endl;
		return (false);
	}

	// bindするよ
	status = bind(socketfd, (const struct sockaddr *)&s_bind, s_bind_siz);
	if (status == -1) {
		std::cout << "bind syscall is failed;;" << std::endl;
		return (false);
	}
	return (true);
}

int main() {
	int socketfd;
	sockaddr_in s_bind;
	socklen_t s_bind_siz;
	int clientfd;
	int status;
	Request *request;
	Response *response;

	//ソケットを作る
	s_bind_siz = sizeof(s_bind);
	status = MakeSocket(socketfd, s_bind);
	if (status == false) std::cout << "MakeSocket failed;;" << std::endl;
	std::cout << "===Start Listening from the socket!" << std::endl;

	// Listen開始します
	listen(socketfd, 2);

	//ループで回します
	while (1) {
		// Accept待ちます
		std::cout << "Waiting for new connection by accept" << std::endl;
		clientfd = accept(socketfd, (struct sockaddr *)&s_bind, &s_bind_siz);

		// Acceptが通ったのでリクエストをReadします
		request = new Request();
		std::cout << "connected. then read" << std::endl
				  << "========" << std::endl
				  << std::endl;
		status = MakeRequest(*request, clientfd);
		if (status == false) std::cout << "MakeRequest Failed;;" << std::endl;

		//クライアントにおくるレスポンスを作る
		response = new Response();
		status = SendResponse(*response, *request, clientfd);
		if (status == false) std::cout << "SendResponse failed;;" << std::endl;

		// FDを閉じて次の接続をまつ
		close(clientfd);
		// delete response;
		// delete request;
		std::cout << std::endl
				  << "========" << std::endl
				  << "close the connection." << std::endl
				  << std::endl;
	}

	return 0;
}
