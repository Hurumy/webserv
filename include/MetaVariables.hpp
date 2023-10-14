/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaVariables.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 20:29:31 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/14 21:24:16 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

class MetaVariables {
	public:
		enum tag {
			AUTH_TYPE,
			CONTENT_LENGTH,
			CONTENT_TYPE,
			GATEWAY_INTERFACE,
			PATH_INFO,
			PATH_TRANSLATED,
			QUERY_STRING,
			REMOTE_ADDR,
			REMOTE_HOST,
			REMOTE_IDENT,
			REMOTE_USER,
			REQUEST_METHOD,
			SCRIPT_NAME,
			SERVER_NAME,
			SERVER_PORT,
			SERVER_PROTOCOL,
			SERVER_SOFTWARE
		};

		void setMetaVar(MetaVariables::tag const varName, std::string const &value);
		void setOptionalMetaVars(std::string const &varName, std::string const &varValue);

	protected:
	private:
		std::string authtype;
		std::string contentLength;
		std::string contentType;
		std::string gateWayInterface;
		std::string pathInfo;
		std::string pathTranslated;
		std::string queryString;
		std::string remoteAddr;
		std::string remoteHost;
		std::string remoteIdent;
		std::string remoteUser;
		std::string requestMethod;
		std::string scriptName;
		std::string serverName;
		std::string serverPort;
		std::string serverProtocol;
		std::string serverSoftware;
		std::map<std::string, std::string> optinalMetaVars;
}
