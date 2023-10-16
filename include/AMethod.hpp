/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMethod.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 10:03:13 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/16 15:34:06 by komatsud         ###   ########.fr       */
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
#include "ConfParser.hpp"

class AMethod {
   private:
   protected:
		AMethod(Config _conf, Request _req, Response &_res);
		~AMethod();
		Config 			conf;
		Request 		req;
		Response 		&res;
		std::string 	uri;
		bool			isloc;
		Location		loc;
		bool								iscgi;
		std::string							path_to_cgi;
		Result<std::string, bool> const _openFile(std::string filename);
		static std::map<unsigned int, std::string> const statusmap;
		static std::map<unsigned int, std::string> initStatusMap();
		// Result<int, bool>					searchSettingsOfURI();
   public:
		virtual Result<int, bool>			act() = 0;
		Result<int, bool>					checkRedirects();
		Result<int, bool>					checkURI();
		void								setURI();
		void								setErrorPageBody();
		Result<std::string, bool> const		isCgi() const;
};
