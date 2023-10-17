/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaVariables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 21:18:00 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/17 15:48:32 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetaVariables.hpp"

void MetaVariables::setMetaVar(MetaVariables::tag const varName,
							   std::string const &value) {
	switch (varName) {
		case MetaVariables::AUTH_TYPE:
			authtype = value;
			break;
		case MetaVariables::CONTENT_LENGTH:
			contentLength = value;
			break;
		case MetaVariables::CONTENT_TYPE:
			contentType = value;
			break;
		case MetaVariables::GATEWAY_INTERFACE:
			gateWayInterface = value;
			break;
		case MetaVariables::PATH_INFO:
			pathInfo = value;
			break;
		case MetaVariables::PATH_TRANSLATED:
			pathTranslated = value;
			break;
		case MetaVariables::QUERY_STRING:
			queryString = value;
			break;
		case MetaVariables::REMOTE_ADDR:
			remoteAddr = value;
			break;
		case MetaVariables::REMOTE_HOST:
			remoteHost = value;
			break;
		case MetaVariables::REMOTE_IDENT:
			remoteIdent = value;
			break;
		case MetaVariables::REMOTE_USER:
			remoteUser = value;
			break;
		case MetaVariables::REQUEST_METHOD:
			requestMethod = value;
			break;
		case MetaVariables::SCRIPT_NAME:
			scriptName = value;
			break;
		case MetaVariables::SERVER_NAME:
			serverName = value;
			break;
		case MetaVariables::SERVER_PORT:
			serverPort = value;
			break;
		case MetaVariables::SERVER_PROTOCOL:
			serverProtocol = value;
			break;
		case MetaVariables::SERVER_SOFTWARE:
			serverSoftware = value;
			break;
	}
}

std::string const &MetaVariables::getMetaVar(MetaVariables::tag const varName) const {
	switch (varName) {
		case MetaVariables::AUTH_TYPE:
			return authtype;
		case MetaVariables::CONTENT_LENGTH:
			return contentLength;
		case MetaVariables::CONTENT_TYPE:
			return contentType;
		case MetaVariables::GATEWAY_INTERFACE:
			return gateWayInterface;
		case MetaVariables::PATH_INFO:
			return pathInfo;
		case MetaVariables::PATH_TRANSLATED:
			return pathTranslated;
		case MetaVariables::QUERY_STRING:
			return queryString;
		case MetaVariables::REMOTE_ADDR:
			return remoteAddr;
		case MetaVariables::REMOTE_HOST:
			return remoteHost;
		case MetaVariables::REMOTE_IDENT:
			return remoteIdent;
		case MetaVariables::REMOTE_USER:
			return remoteUser;
		case MetaVariables::REQUEST_METHOD:
			return requestMethod;
		case MetaVariables::SCRIPT_NAME:
			return scriptName;
		case MetaVariables::SERVER_NAME:
			return serverName;
		case MetaVariables::SERVER_PORT:
			return serverPort;
		case MetaVariables::SERVER_PROTOCOL:
			return serverProtocol;
		case MetaVariables::SERVER_SOFTWARE:
			return serverSoftware;
	}
}

void MetaVariables::setOptionalMetaVars(std::string const &varName,
										std::string const &varValue) {
	optinalMetaVars.insert(std::make_pair(varName, varValue));
}
