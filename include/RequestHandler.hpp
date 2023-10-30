/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:25:16 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/29 16:42:47 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

class RequestHandler {
	private:
		std::vector<Config> const &configs;
		Request req;
		Response res;
		size_t confnum;
		size_t addressnum;
		std::string servername;
		std::string query;
		bool iscgi;
		std::string path_to_cgi;
		Result<std::string, bool> _openFile(std::string filename);

	protected:
	public:
		RequestHandler(std::vector<Config> const &_conf, Request const _req);
		Result<int, bool> searchMatchHost();
		Result<int, bool> checkRequiedHeader();
		Result<int, bool> routeMethod();
		void setErrorPageBody();
		Response getResponse();
		Result<std::string, bool> const isCgi() const;
		std::string const getHostname() const;
		std::string const &getQuery() const;
		int getPortNumber() const;
};
