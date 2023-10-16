/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 11:46:18 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 10:59:50 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

std::string const Location::getUri() const {return (this->uri);}

std::string const Location::getRootDir() const { return (this->rootdir); }

unsigned long long Location::getMaxBodySize() const {
	return (this->maxbodysize);
}

Result<std::string, bool> const Location::getErrorPages(int key) const {
	if (errorpages.empty() == true) {
		return Error<bool>(false);
	} else if (errorpages.find(key) == errorpages.end()) {
		return Error<bool>(false);
	} else
		return Ok<std::string>(errorpages.at(key));
}

Result<std::string, bool> const Location::getRedirects(std::string key) const {
	if (this->redirects.empty() == true) {
		return Error<bool>(false);
	} else if (this->redirects.find(key) == this->redirects.end()) {
		return Error<bool>(false);
	} else
		return Ok<std::string>(this->redirects.at(key));
}

bool Location::isReturn() const { return (this->isreturn); }

int Location::getReturnStatus() const { return (this->returnstatus); }

std::string Location::getReturnUrl() const { return (this->returnurl); }

std::string	Location::getReturnBody() const
{
	return (returnbody);
}

bool Location::getDirlist() const { return (this->dirlisting); }

std::vector<std::string> const Location::getIndex() const {
	return (this->index);
}

std::string const Location::getUploadPath() const { return (this->uploadpath); }

Result<std::string, bool> const Location::getReqMethod(std::string key) const {
	if (this->reqmethod.empty() == true)
		return Ok<std::string>("");
	else if (reqmethod.find(key) == reqmethod.end()) {
		return Error<bool>(false);
	} else
		return Ok<std::string>("");
}

Result<int, bool> const	Location::getCgiExtension(std::string _ext) const
{
	for (size_t i = 0; i < cgiextension.size(); i ++)
	{
		if (cgiextension.at(i) == _ext)
		{
			return Ok<int>(0);
		}
	}
	return Error<bool>(false);
}

bool Location::setUri(std::string _uri)
{
	this->uri = _uri;
	return (true);
}


bool Location::setRootDir(std::string _root) {
	rootdir = _root;
	return (true);
}

bool Location::setMaxBodySize(unsigned long long siz) {
	this->maxbodysize = siz;
	return (true);
}

bool Location::addErrorPages(int key, std::string val) {
	this->errorpages[key] = val;
	return (true);
}

bool Location::addRedirects(std::string key, std::string val) {
	this->redirects[key] = val;
	return (true);
}

bool Location::setIsReturn(bool is) {
	this->isreturn = is;
	return (true);
}

bool Location::setReturnStatus(int status) {
	this->returnstatus = status;
	return (true);
}

bool Location::setReturnUrl(std::string url) {
	this->returnurl = url;
	return (true);
}

bool Location::setReturnBody(std::string _body)
{
	returnbody = _body;
	return (true);
}

bool Location::setDirlist(bool conf) {
	this->dirlisting = conf;
	return (true);
}

bool Location::addIndex(std::string _index) {
	this->index.push_back(_index);
	return (true);
}

bool Location::setUploadPath(std::string path) {
	this->uploadpath = path;
	return (true);
}

bool Location::addReqMethod(std::string key, bool val) {
	this->reqmethod[key] = val;
	return (true);
}

bool Location::addCgiExtension(std::string _ext)
{
	cgiextension.push_back(_ext);
	return (true);
}



