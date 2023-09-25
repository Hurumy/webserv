/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet_test.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:25:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 10:33:46 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "webserv.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "AMethod.hpp"
#include "MethodGet.hpp"

TEST(MethodGetTest, NormalTest)
{
	Result<std::vector<Config>, bool> res = parseConf(CONF_FILE_PATH);
	std::vector<Config>			tmp = res.getOk();
	Request						req;
	bool						expected(false);
	unsigned int				expected_status(505);
	std::string					expected_string("HTTP Version Not Supported");
	std::string					expected_body("<H1>HTTP 505 HTTP Version Not Supported</H1>\n\n<p>This webserv supports only HTTP/1.1...</p>");
	std::string					expected_content_length("91");

	req.setVersion("HTTP/2.0");
	req.setMethod("GET");
	req.addHeader("Host", "_");

	RequestHandler handler = RequestHandler(tmp, req);
	handler.searchMatchHost();
	Result<int, bool> result_1 = handler.checkRequiedHeader();
	ASSERT_EQ(result_1.isOK(), expected);

	//errorPageBody
	handler.setErrorPageBody();
	Result<std::string, bool>	res_1 = handler.getResponse().getHeader("Content-Length");
	std::string					content_len = res_1.getOk();

	ASSERT_EQ(handler.getResponse().getStatus(), expected_status);
	ASSERT_EQ(handler.getResponse().getStatusMessage(), expected_string);
	ASSERT_EQ(handler.getResponse().getBody(), expected_body);
	ASSERT_EQ(content_len, expected_content_length);
}


