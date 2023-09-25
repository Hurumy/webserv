/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost_test.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:52:26 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 14:11:38 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "MethodPost.hpp"
#include "RequestHandler.hpp"
#include "ConfParser.hpp"

#define	CONF_FILE_PATH "testconfs/method_post.conf"

static std::string	__openFile(std::string filename)
{
	int 				fd;
	unsigned long long	bodysize = 0;
	int 				status = 1;
	std::string 		body;
	char 				buf[FILE_READ_SIZE];

	// open
	fd = open(filename.c_str(), O_RDONLY);
	if (fd < 0)
		return ("Open Failed");

	// read
	while (status > 0) {
		status = read(fd, buf, FILE_READ_SIZE);
		buf[status] = '\0';
		if (status != -1)
		{
			body += buf;
			bodysize += status;
		}
	}
	close(fd);

	if (status == -1)
		return ("close or read failed");

	//Bodyの読み込みが成功していたら、bodysizeとBodyをセットして返る
	return (body);
}

TEST (MethodPostTest, postTextFileTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(201);
	std::string					expected_string("Created");
	bool						expected_status_location(true);
	std::string					expected_content("post file!!");

	req.setVersion("HTTP/1.1");
	req.setMethod("POST");
	req.addHeader("Host", "_");
	req.setBody(expected_content);
	req.addHeader("Content-Length", "11");
	req.setUrl("./post");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getHeader("Location").isOK(), expected_status_location);
	
	Result<std::string, bool> res_loc = handler.getResponse().getHeader("Location");
	std::string	filename = res_loc.getOk();
	std::string body = __openFile(filename);

	ASSERT_EQ(body, expected_content);
}

TEST (MethodPostTest, postTextFileTest_Error_methodNotAllowed)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(405);
	std::string					expected_string("Method Not Allowed");
	std::string					expected_content("huowadwasdhjwahjdkwhujwaduhjadwuhjdwauij");

	req.setVersion("HTTP/1.1");
	req.setMethod("POST");
	req.addHeader("Host", "met_not_allowed.com");
	req.setBody(expected_content);
	req.addHeader("Content-Length", "40");
	req.setUrl("./post");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}