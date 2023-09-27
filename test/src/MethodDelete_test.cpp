/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodDelete_test.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:23:41 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/27 19:10:01 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>

#include "MethodDelete.hpp"
#include "RequestHandler.hpp"
#include "ConfParser.hpp"

#define	CONF_FILE_PATH "testconfs/method_delete.conf"

TEST (MethodDeleteTest, deletePostedFileTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(204);
	std::string					expected_string("No Content");

	req.setVersion("HTTP/1.1");
	req.setMethod("DELETE");
	req.addHeader("Host", "_");
	req.setUrl("/post/0");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}

TEST (MethodDeleteTest, deletePostedFileTest_Error_NotFound)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(true);
	unsigned int				expected_status(404);
	std::string					expected_string("Not Found");

	req.setVersion("HTTP/1.1");
	req.setMethod("DELETE");
	req.addHeader("Host", "_");
	req.setUrl("/dummy/thereisnofile.html");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	handler.routeMethod();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
}
