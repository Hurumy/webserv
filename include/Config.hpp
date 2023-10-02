/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:00:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/02 18:03:21 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "Address.hpp"


class	Config
{
	private:
		std::vector<Address>						addresses;
		std::vector<std::string>					servername;
		std::string									rootdir;
		unsigned long long							maxbodysize;
		std::map<int, std::string>					errorpages;
		std::map<std::string, std::string>			redirects;
		bool										isreturn;
		int											returnstatus;
		std::string									returnurl;
		bool										dirlisting;
		std::vector<std::string>					index;
		std::string									uploadpath;
		std::map<std::string, bool>					reqmethod;
	
	protected:
	
	public:
		std::vector<Address> const			getAddresses() const;
		std::vector<std::string> const		getServerName() const;
		std::string const					getRootDir() const;
		unsigned long long					getMaxBodySize() const;
		Result<std::string, bool> const		getErrorPages(int status) const;
		Result<std::string, bool> const		getRedirects(std::string key) const;
		bool								isReturn() const;
		int									getReturnStatus() const;
		std::string							getReturnUrl() const;
		bool								getDirlist() const;
		std::vector<std::string> const		getIndex() const;
		std::string const					getUploadPath() const;
		Result<std::string, bool> const		getReqMethod(std::string key) const;
		
		//for test
		bool								addAddresses(Address &add);
		bool								addServerName(std::string name);
		bool								setRootDir(std::string root);
		bool								setMaxBodySize(unsigned long long siz);
		bool								addErrorPages(int key, std::string val);
		bool								addRedirects(std::string key, std::string val);
		bool								setIsReturn(bool is);
		bool								setReturnStatus(int status);
		bool								setReturnUrl(std::string url);
		bool								setDirlist(bool conf);
		bool								addIndex(std::string _index);
		bool								setUploadPath(std::string path);
		bool								addReqMethod(std::string key, bool val);
};

