/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 09:00:44 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/20 15:48:02 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Address.hpp"
#include "Error.hpp"
#include "Location.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "webserv.hpp"

class Config {
	private:
		std::vector<Address> addresses;
		std::map<std::string, Location> locations;
		std::vector<std::string> servername;
		std::string rootdir;
		unsigned long long maxbodysize;
		std::map<int, std::string> errorpages;
		std::map<std::string, std::string> redirects;
		bool isreturn;
		int returnstatus;
		std::string returnurl;
		std::string returnbody;
		bool dirlisting;
		std::vector<std::string> index;
		std::string uploadpath;
		std::map<std::string, bool> reqmethod;
		std::vector<std::string> cgiextension;

	protected:
	public:
		std::vector<Address> const &getAddresses() const;
		Result<Location, bool> const getLocations(std::string key) const;
		std::vector<std::string> const getServerName() const;
		std::string const getRootDir() const;
		unsigned long long getMaxBodySize() const;
		Result<std::string, bool> const getErrorPages(int status) const;
		Result<std::string, bool> const getRedirects(std::string key) const;
		bool isReturn() const;
		int getReturnStatus() const;
		std::string getReturnUrl() const;
		std::string getReturnBody() const;
		bool getDirlist() const;
		std::vector<std::string> const getIndex() const;
		std::string const getUploadPath() const;
		Result<std::string, bool> const getReqMethod(std::string key) const;
		Result<int, bool> const getCgiExtension(std::string _ext) const;

		// for test
		bool addAddresses(Address &add);
		bool addLocations(std::string key, Location val);
		bool addServerName(std::string name);
		bool setRootDir(std::string root);
		bool setMaxBodySize(unsigned long long siz);
		bool addErrorPages(int key, std::string val);
		bool addRedirects(std::string key, std::string val);
		bool setIsReturn(bool is);
		bool setReturnStatus(int status);
		bool setReturnUrl(std::string url);
		bool setReturnBody(std::string _body);
		bool setDirlist(bool conf);
		bool addIndex(std::string _index);
		bool setUploadPath(std::string path);
		bool addReqMethod(std::string key, bool val);
		bool addCgiExtension(std::string _ext);

		// for copy
		std::map<int, std::string> pullErrorPages() const;
		bool pushErrorPages(std::map<int, std::string> map);
};
