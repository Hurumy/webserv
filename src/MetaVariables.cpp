/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaVariables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 21:18:00 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/14 21:33:29 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetaVariables.hpp"

void MetaVariables::setMetaVar(MetaVariables::tag const varName, std::string const &value) {
	switch (varName) {
		case MetaVariables::AUTH_TYPE:
			authtype = value;
			break ;
		case MetaVariables::CONTENT_LENGTH:
			contentLength = value;
			break ;
		case MetaVariables::CONTENT_TYPE:
			contentType = value;
			break ;
		case MetaVariables::GATEWAY_INTERFACE:
			gateWayInterface = value;
			break ;
		case MetaVariables::PATH_INFO:
			pathInfo = value;
			break ;
		case MetaVariables::PATH_TRANSLATED:
			pathTranslated = value;
			break ;
		case MetaVariables::QUERY_STRING:
			queryString = value;
			break ;
		case MetaVariables::REMOTE_ADDR:
			remoteAddr = value;
			break ;
		case MetaVariables::REMOTE_HOST:
			remoteHost = value;
			break ;
		case MetaVariables::REMOTE_IDENT:
			remoteIdent = value;
			break ;
		case MetaVariables::REMOTE_USER:
			remoteUser = value;
			break ;
		case MetaVariables::REQUEST_METHOD:
			requestMethod = value;
			break ;
		case MetaVariables::SCRIPT_NAME:
			scriptName = value;
			break ;
		case MetaVariables::SERVER_NAME:
			serverName = value;
			break ;
		case MetaVariables::SERVER_PORT:
			serverPort = value;
			break ;
		case MetaVariables::SERVER_PROTOCOL:
			serverProtocol = value;
			break ;
		case MetaVariables::SERVER_SOFTWARE:
			serverSoftware = value;
			break ;
	}
}

void MetaVariables::setOptionalMetaVars(std::string const &varName, std::string const &varValue) {
	optinalMetaVars.insert(std::make_pair(varName, varValue));
}
