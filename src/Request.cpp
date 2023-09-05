/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:54:10 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/05 11:05:44 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

const std::string Request::getLines() const {
	std::string line;

	line += this->version;
	line += this->body;
	line += this->url;
	return (line);
}

bool Request::setUrl(std::string url) {
	this->url = url;
	return (true);
}

std::string const Request::getUrl(void) const { return (this->url); }

bool Request::setMethod(std::string method) {
	this->method = method;
	return (true);
}

std::string const Request::getMethod(void) const { return (this->method); }

// test code
/*
int main()
{
	Request *test = new Request();

	test->setversion("v1.2\n");
	test->seturl("http:\n");
	test->setbody("NICE BODY~\n");

	std::cout << test->getLines() << std::endl;
	std::cout << test->getUrl() << std::endl;
	std::cout << test->getVersion() << std::endl;
	std::cout << test->getBody() << std::endl;

	delete test;
	return (0);
}*/
