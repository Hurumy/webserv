/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:32:21 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/18 10:47:41 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

#include "AMethod.hpp"
#include "MethodDelete.hpp"
#include "MethodGet.hpp"
#include "MethodPost.hpp"

RequestHandler::RequestHandler(std::vector<Config> const &_conf,
							   Request const _req)
	: configs(_conf) {
	this->req = _req;
	this->res = Response();
}

// configs.at(i) の i を返す
// まだホストが見つかってもいないので、エラーページをセットすることが不可能。ステータスのみ返す
Result<int, bool> RequestHandler::searchMatchHost() {
	Result<std::string, bool> result_1 = req.getHeader("Host");
	std::string hostname;
	std::string without_port;
	bool portflag;
	int portnum;

	//レスポンスに最低限をセットする
	res.setVersion("HTTP/1.1");

	// Hostヘッダー自体が含まれていない場合(どうにもならない)
	if (result_1.isOK() == false) {
		res.setStatus(400);
		res.setStatusMessage("Bad Request");
		res.addHeader("Content-Length", "0");
		return Error<bool>(false);
	}

	hostname = result_1.getOk();

	// std::cout << YELLOW << "hostname: " << hostname << RESET << std::endl;

	// Requestで指定されているhostnameの中にPortの指定が存在するかどうかを確認する
	if (hostname.find(':') != std::string::npos) {
		std::stringstream ss;

		portflag = true;
		ss << hostname;
		std::getline(ss, without_port, ':');
		// std::cout << "hostname: " << hostname << std::endl;
		// std::cout << "without_port: " << without_port << std::endl;
		ss >> portnum;
		// std::cout << portnum << std::endl;
	} else {
		portflag = false;
	}

	// Configの中からHostが一致するものを探す
	for (size_t i = 0; i < configs.size(); i++)
	{
		for (size_t t = 0; t < configs.at(i).getServerName().size(); t++)
		{
			if (portflag == false && configs.at(i).getServerName().at(t) == hostname)
			{
				this->confnum = i;
				res.addHeader("Server", configs.at(i).getServerName().at(t));
				return Ok<int>(i);
			}
			else if (portflag == true && configs.at(i).getServerName().at(t) == without_port)
			{
				//そのサーバーネームに対してポートが合っているか確認する
				for (size_t j = 0; j < configs.at(i).getAddresses().size(); j++)
				{
					if (configs.at(i).getAddresses().at(j).getPort() == portnum)
					{
						this->addressnum = j;
						this->confnum = i;
						this->servername = configs.at(i).getServerName().at(t);
						res.addHeader("Server", configs.at(i).getServerName().at(t));
						return Ok<int>(i);
					}
				}
			}
		}
	}
	res.setStatus(400);
	res.setStatusMessage("Bad Request");
	res.addHeader("Content-Length", "0");
	return Error<bool>(false);
}

Result<int, bool> RequestHandler::checkRequiedHeader() {
	if (req.getVersion() != "HTTP/1.1") {
		res.setStatus(505);
		res.setStatusMessage("HTTP Version Not Supported");
		setErrorPageBody();
		return Error<bool>(false);
	}
	if (this->confnum < configs.size() &&
		configs.at(confnum).getReqMethod(req.getMethod()).isOK() == false) {
		res.setStatus(405);
		res.setStatusMessage("Method Not Allowed");
		setErrorPageBody();
		return Error<bool>(false);
	}
	return Ok<int>(0);
}

//ここで、各Method内でエラーが見つかった時にはその中でエラーページをセットしている
Result<int, bool> RequestHandler::routeMethod()
{
	if (req.getMethod() == "GET")
	{
		//クラス呼ぶ
		MethodGet get(configs.at(confnum), req, res);

		// URIチェック
		Result<int, bool> res_uri = get.checkURI();
		if (res_uri.isOK() == false) {
			setErrorPageBody();
			return Error<bool>(false);
		}
		get.setURI();

		//cgiだったらcgiの情報をセットして返す
		if (get.isCgi().isOK() == true) {
			iscgi = true;
			path_to_cgi = get.isCgi().getOk();
		} else {
			iscgi = false;
		} 

		//リダイレクトチェック
		Result<int, bool> res_rg = get.checkRedirects();
		if (res_rg.isOK() == true) return Ok<int>(0);

		// getする
		Result<int, bool> res_get = get.act();
		if (res_get.isOK() == false)
			return Error<bool>(false);
		else
			return Ok<int>(0);
	}
	else if (req.getMethod() == "POST")
	{
		//クラス呼ぶ
		MethodPost post(configs.at(confnum), req, res);

		// URIチェック
		Result<int, bool> res_uri = post.checkURI();
		if (res_uri.isOK() == false) {
			setErrorPageBody();
			return Error<bool>(false);
		}
		post.setURI();

		//cgiだったらcgiの情報をセットして返す
		if (post.isCgi().isOK() == true) {
			iscgi = true;
			path_to_cgi = post.isCgi().getOk();
		} else {
			iscgi = false;
		}

		//リダイレクトチェック
		Result<int, bool> res_rp = post.checkRedirects();
		if (res_rp.isOK() == true) return Ok<int>(0);

		// POSTする
		Result<int, bool> res_post = post.act();
		if (res_post.isOK() == false)
			return Error<bool>(false);
		else
			return Ok<int>(0);

	} else if (req.getMethod() == "DELETE") {
		//クラス呼ぶ
		MethodDelete del(configs.at(confnum), req, res);

		// URIチェック
		Result<int, bool> res_uri = del.checkURI();
		if (res_uri.isOK() == false) {
			setErrorPageBody();
			return Error<bool>(false);
		}
		del.setURI();

		//cgiだったらcgiの情報をセットして返す
		if (del.isCgi().isOK() == true) {
			iscgi = true;
			path_to_cgi = del.isCgi().getOk();
		} else {
			iscgi = false;
		}

		//リダイレクトチェック
		Result<int, bool> res_rd = del.checkRedirects();
		if (res_rd.isOK() == true) return Ok<int>(0);

		// DELETEする
		Result<int, bool> res_del = del.act();
		if (res_del.isOK() == false)
			return Error<bool>(false);
		else
			return Ok<int>(0);
	} else {
		// Methodがこの3つ以外だった場合、Not Allowedを返す
		res.setStatus(405);
		res.setStatusMessage("Method Not Allowed");
		setErrorPageBody();
		return Error<bool>(false);
	}
}

Result<std::string, bool> RequestHandler::_openFile(std::string filename) {
	int fd;
	unsigned long long bodysize = 0;
	int status = 1;
	std::string body;
	char buf[FILE_READ_SIZE];

	// open
	fd = open(filename.c_str(), O_RDONLY);
	if (fd == -1 && errno == ENOENT) {
		res.setStatus(404);
		res.setStatusMessage("Not Found");
		return Error<bool>(false);
	} else if (fd == -1 && errno == EACCES) {
		res.setStatus(403);
		res.setStatusMessage("Forbidden");
		return Error<bool>(false);
	}

	// read
	while (status > 0) {
		status = read(fd, buf, FILE_READ_SIZE);
		buf[status] = '\0';
		if (status != -1) {
			body += buf;
			bodysize += status;
		}
	}
	close(fd);

	if (status == -1) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return Error<bool>(false);
	}

	// Bodyの読み込みが成功していたら、bodysizeとBodyをセットして返る
	std::stringstream ss;
	std::string length;
	ss << bodysize;
	ss >> length;
	res.addHeader("Content-Length", length);
	res.setBody(body);
	return Ok<std::string>(body);
}

void RequestHandler::setErrorPageBody()
{
	unsigned int prevstatus = res.getStatus();
	Result<std::string, bool> res_1 =
		configs.at(confnum).getErrorPages(res.getStatus());

	if (res_1.isOK() == false) {
		res.addHeader("Content-Length", "0");
		return;
	}

	std::string filename = res_1.getOk();

	// bodyをセットする。成功したら抜けるループ
	// bodyのセットに失敗した場合は、失敗した後のステータスが変わっていればもう一度ファイルを検索する
	//変わっていなければBodyなしでヘッダだけ送付する
	while (1) {
		Result<std::string, bool> res_2 = _openFile(filename);
		if (res_2.isOK() == true)
			break;
		else if (res_2.isOK() == false && prevstatus != res.getStatus()) {
			prevstatus = res.getStatus();
			Result<std::string, bool> res_3 =
				configs.at(confnum).getErrorPages(res.getStatus());
			if (res_3.isOK() == false) {
				res.addHeader("Content-Length", "0");
				break;
			}
			filename = res_3.getOk();
		} else {
			res.addHeader("Content-Length", "0");
			break;
		}
	}
	return ;
}

Response RequestHandler::getResponse() { return (this->res); }

Result<std::string, bool> const RequestHandler::isCgi() const {
	if (iscgi == true) {
		return Ok<std::string>(path_to_cgi);
	}
	return Error<bool>(false);
}

std::string const	RequestHandler::getHostname() const
{
	return (this->servername);
}

int const			RequestHandler::getPortNumber() const
{
	return (configs.at(confnum).getAddresses().at(addressnum).getPort());
}

