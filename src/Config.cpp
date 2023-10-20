/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:11:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/20 12:27:15 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::vector<Address> const Config::getAddresses() const {
	return (this->addresses);
}

std::vector<Address> const &Config::getAddressesReference() const {
	return (this->addresses);
}

Result<Location, bool> const Config::getLocations(std::string _path) const {
	if (locations.empty() == true) {
		return Error<bool>(false);
	} else if (locations.find(_path) == locations.end()) {
		return Error<bool>(false);
	} else
		return Ok<Location>(locations.at(_path));
}

std::vector<std::string> const Config::getServerName() const {
	return (this->servername);
}

std::string const Config::getRootDir() const { return (this->rootdir); }

unsigned long long Config::getMaxBodySize() const {
	return (this->maxbodysize);
}

Result<std::string, bool> const Config::getErrorPages(int key) const {
	if (errorpages.empty() == true) {
		return Error<bool>(false);
	} else if (errorpages.find(key) == errorpages.end()) {
		return Error<bool>(false);
	} else
		return Ok<std::string>(errorpages.at(key));
}

Result<std::string, bool> const Config::getRedirects(std::string key) const {
	if (this->redirects.empty() == true) {
		return Error<bool>(false);
	} else if (this->redirects.find(key) == this->redirects.end()) {
		return Error<bool>(false);
	} else
		return Ok<std::string>(this->redirects.at(key));
}

bool Config::isReturn() const { return (this->isreturn); }

int Config::getReturnStatus() const { return (this->returnstatus); }

std::string Config::getReturnUrl() const { return (this->returnurl); }

std::string Config::getReturnBody() const { return (returnbody); }

bool Config::getDirlist() const { return (this->dirlisting); }

std::vector<std::string> const Config::getIndex() const {
	return (this->index);
}

std::string const Config::getUploadPath() const { return (this->uploadpath); }

Result<std::string, bool> const Config::getReqMethod(std::string key) const {
	if (this->reqmethod.empty() == true)
		return Ok<std::string>("");
	else if (reqmethod.find(key) == reqmethod.end()) {
		return Error<bool>(false);
	} else
		return Ok<std::string>("");
}

Result<int, bool> const Config::getCgiExtension(std::string _ext) const {
	for (size_t i = 0; i < cgiextension.size(); i++) {
		if (cgiextension.at(i) == _ext) {
			return Ok<int>(0);
		}
	}
	return Error<bool>(false);
}

bool Config::addAddresses(Address &add) {
	this->addresses.push_back(add);
	return (true);
}

bool Config::addServerName(std::string name) {
	this->servername.push_back(name);
	return (true);
}

bool Config::addLocations(std::string const key, Location const val) {
	if (locations.find(key) == locations.end()) {
		this->locations[key] = val;
		return (true);
	} else {
		return (false);
	}
}

bool Config::setRootDir(std::string root) {
	this->rootdir = root;
	return (true);
}

bool Config::setMaxBodySize(unsigned long long siz) {
	this->maxbodysize = siz;
	return (true);
}

bool Config::addErrorPages(int key, std::string val) {
	this->errorpages[key] = val;
	return (true);
}

bool Config::addRedirects(std::string key, std::string val) {
	this->redirects[key] = val;
	return (true);
}

bool Config::setIsReturn(bool is) {
	this->isreturn = is;
	return (true);
}

bool Config::setReturnStatus(int status) {
	this->returnstatus = status;
	return (true);
}

bool Config::setReturnUrl(std::string url) {
	this->returnurl = url;
	return (true);
}

bool Config::setReturnBody(std::string _body) {
	returnbody = _body;
	return (true);
}

bool Config::setDirlist(bool conf) {
	this->dirlisting = conf;
	return (true);
}

bool Config::addIndex(std::string _index) {
	this->index.push_back(_index);
	return (true);
}

bool Config::setUploadPath(std::string path) {
	this->uploadpath = path;
	return (true);
}

bool Config::addReqMethod(std::string key, bool val) {
	if (reqmethod.find(key) == reqmethod.end()) {
		this->reqmethod[key] = val;
		return (true);
	} else {
		return (false);
	}
}

bool Config::addCgiExtension(std::string _ext) {
	cgiextension.push_back(_ext);
	return (true);
}
