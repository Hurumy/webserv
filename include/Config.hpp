/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:00:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/08 11:32:22 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"


class	Config
{
	private:
		int											port;
		std::string									servername;
		std::string									rootdir;
		unsigned long long							maxbodysize;
		std::map<int, std::string>					errorpages;
		std::map<std::string, std::string>			redirects;
		bool										dirlisting;
		std::string									returndir;
		std::string									uploadpath;
		std::map<std::string, bool>					reqmethod;
	
	protected:
	
	public:
		int 								getPort() const;
		std::string const					getServerName() const;
		std::string const					getRootDir() const;
		unsigned long long					getMaxBodySize() const;
		Result<std::string, bool> const		getErrorPages(int status) const;
		Result<std::string, bool> const		getRedirects(std::string key) const;
		bool								getDirlist() const;
		std::string const					getReturnDir() const;
		std::string const					getUploadPath() const;
		Result<std::string, bool> const		getReqMethod(std::string key) const;
		
		//for test
		bool								setPort(int port);
		bool								setServerName(std::string name);
		bool								setRootDir(std::string root);
		bool								setMaxBodySize(unsigned long long siz);
		bool								addErrorPages(int key, std::string val);
		bool								addRedirects(std::string key, std::string val);
		bool								setDirlist(bool conf);
		bool								setReturnDir(std::string retdir);
		bool								setUploadPath(std::string path);
		bool								addReqMethod(std::string key, bool val);
};

