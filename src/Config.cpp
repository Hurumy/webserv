/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:11:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 10:08:59 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "Result.hpp"
#include "Config.hpp"

int	Config::getPort() const{return(this->port);}

std::string const	Config::getServerName() const {return(this->servername);}

std::string const	Config::getRootDir() const {return(this->rootdir);}

unsigned long long	Config::getMaxBodySize() const {return(this->maxbodysize);}

Result<std::string, bool> const	Config::getErrorPages(int key) const
{
	if (errorpages.empty() == true)
	{
		return Error<bool>(false);
	}
	else if (errorpages.find(key) == errorpages.end())
	{
		return Error<bool>(false);
	}
	else
		return Ok<std::string>(errorpages.at(key));
}

Result<std::string, bool> const	Config::getRedirects(std::string key) const
{
	if(this->redirects.empty() == true)
	{
		return Error<bool>(false);
	}
	else if (this->redirects.find(key) == this->redirects.end())
	{
		return Error<bool> (false);
	}
	else
		return Ok<std::string>(this->redirects.at(key));
}

bool	Config::getDirlist() const
{return(this->dirlisting);}

std::string const	Config::getReturnDir() const
{return(this->returndir);}

std::string const	Config::getUploadPath() const
{return(this->uploadpath);}

Result<std::string, bool> const	Config::getReqMethod(std::string key) const
{
	if (this->reqmethod.empty() == true)
		return Ok<std::string>("");
	else if (this->reqmethod.find(key) == this->reqmethod.end())
	{
		return Error<bool>(false);
	}
	else
		return Ok<std::string>("");
}

bool	Config::setPort(int port)
{
	this->port = port;
	return (true);
}

bool	Config::setServerName(std::string name)
{
	this->servername = name;
	return (true);
}

bool	Config::setRootDir(std::string root)
{
	this->rootdir = root;
	return (true);
}

bool	Config::setMaxBodySize(unsigned long long siz)
{
	this->maxbodysize = siz;
	return (true);
}

bool	Config::addErrorPages(int key, std::string val)
{
	this->errorpages[key] = val;
	return (true);
}

bool	Config::addRedirects(std::string key, std::string val)
{
	this->redirects[key] = val;
	return (true);
}

bool	Config::setDirlist(bool conf)
{
	this->dirlisting = conf;
	return (true);
}

bool	Config::setReturnDir(std::string retdir)
{
	this->returndir = retdir;
	return (true);
}

bool	Config::setUploadPath(std::string path)
{
	this->uploadpath = path;
	return (true);
}

bool	Config::addReqMethod(std::string key, bool val)
{
	this->reqmethod[key] = val;
	return (true);
}



