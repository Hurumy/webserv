/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaVariables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 21:18:00 by shtanemu          #+#    #+#             */
/*   Updated: 2023/11/06 16:03:45 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetaVariables.hpp"

MetaVariables::MetaVariables() : metaVariablesMap(initMetaVariables()) {}

std::map<std::string, std::string> MetaVariables::initMetaVariables() {
	std::map<std::string, std::string> initialMetaVariables;

	initialMetaVariables["AUTH_TYPE="] = "";
	initialMetaVariables["CONTENT_LENGTH="] = "";
	initialMetaVariables["CONTENT_TYPE="] = "";
	initialMetaVariables["GATEWAY_INTERFACE="] = "";
	initialMetaVariables["PATH_INFO="] = "";
	initialMetaVariables["PATH_TRANSLATED="] = "";
	initialMetaVariables["QUERY_STRING="] = "";
	initialMetaVariables["REMOTE_ADDR="] = "";
	initialMetaVariables["REMOTE_HOST="] = "";
	initialMetaVariables["REMOTE_IDENT="] = "";
	initialMetaVariables["REMOTE_USER="] = "";
	initialMetaVariables["REQUEST_METHOD="] = "";
	initialMetaVariables["SCRIPT_NAME="] = "";
	initialMetaVariables["SERVER_PORT="] = "";
	initialMetaVariables["SERVER_NAME="] = "";
	initialMetaVariables["SERVER_PROTOCOL="] = "";
	initialMetaVariables["SERVER_SOFTWARE="] = "";
	return initialMetaVariables;
}

void MetaVariables::setMetaVar(MetaVariables::tag const varName,
							   std::string const &value) {
	switch (varName) {
		case MetaVariables::AUTH_TYPE:
			metaVariablesMap["AUTH_TYPE="] = value;
			break;
		case MetaVariables::CONTENT_LENGTH:
			metaVariablesMap["CONTENT_LENGTH="] = value;
			break;
		case MetaVariables::CONTENT_TYPE:
			metaVariablesMap["CONTENT_TYPE="] = value;
			break;
		case MetaVariables::GATEWAY_INTERFACE:
			metaVariablesMap["GATEWAY_INTERFACE="] = value;
			break;
		case MetaVariables::PATH_INFO:
			metaVariablesMap["PATH_INFO="] = value;
			break;
		case MetaVariables::PATH_TRANSLATED:
			metaVariablesMap["PATH_TRANSLATED="] = value;
			break;
		case MetaVariables::QUERY_STRING:
			metaVariablesMap["QUERY_STRING="] = value;
			break;
		case MetaVariables::REMOTE_ADDR:
			metaVariablesMap["REMOTE_ADDR="] = value;
			break;
		case MetaVariables::REMOTE_HOST:
			metaVariablesMap["REMOTE_HOST="] = value;
			break;
		case MetaVariables::REMOTE_IDENT:
			metaVariablesMap["REMOTE_IDENT="] = value;
			break;
		case MetaVariables::REMOTE_USER:
			metaVariablesMap["REMOTE_USER="] = value;
			break;
		case MetaVariables::REQUEST_METHOD:
			metaVariablesMap["REQUEST_METHOD="] = value;
			break;
		case MetaVariables::SCRIPT_NAME:
			metaVariablesMap["SCRIPT_NAME="] = value;
			break;
		case MetaVariables::SERVER_NAME:
			metaVariablesMap["SERVER_NAME="] = value;
			break;
		case MetaVariables::SERVER_PORT:
			metaVariablesMap["SERVER_PORT="] = value;
			break;
		case MetaVariables::SERVER_PROTOCOL:
			metaVariablesMap["SERVER_PROTOCOL="] = value;
			break;
		case MetaVariables::SERVER_SOFTWARE:
			metaVariablesMap["SERVER_SOFTWARE="] = value;
			break;
	}
}

std::string const &MetaVariables::getMetaVar(MetaVariables::tag const varName) {
	switch (varName) {
		case MetaVariables::AUTH_TYPE:
			return metaVariablesMap["AUTH_TYPE="];
		case MetaVariables::CONTENT_LENGTH:
			return metaVariablesMap["CONTENT_LENGTH="];
		case MetaVariables::CONTENT_TYPE:
			return metaVariablesMap["CONTENT_TYPE="];
		case MetaVariables::GATEWAY_INTERFACE:
			return metaVariablesMap["GATEWAY_INTERFACE="];
		case MetaVariables::PATH_INFO:
			return metaVariablesMap["PATH_INFO="];
		case MetaVariables::PATH_TRANSLATED:
			return metaVariablesMap["PATH_TRANSLATED="];
		case MetaVariables::QUERY_STRING:
			return metaVariablesMap["QUERY_STRING="];
		case MetaVariables::REMOTE_ADDR:
			return metaVariablesMap["REMOTE_ADDR="];
		case MetaVariables::REMOTE_HOST:
			return metaVariablesMap["REMOTE_HOST="];
		case MetaVariables::REMOTE_IDENT:
			return metaVariablesMap["REMOTE_IDENT="];
		case MetaVariables::REMOTE_USER:
			return metaVariablesMap["REMOTE_USER="];
		case MetaVariables::REQUEST_METHOD:
			return metaVariablesMap["REQUEST_METHOD="];
		case MetaVariables::SCRIPT_NAME:
			return metaVariablesMap["SCRIPT_NAME="];
		case MetaVariables::SERVER_NAME:
			return metaVariablesMap["SERVER_NAME="];
		case MetaVariables::SERVER_PORT:
			return metaVariablesMap["SERVER_PORT="];
		case MetaVariables::SERVER_PROTOCOL:
			return metaVariablesMap["SERVER_PROTOCOL="];
		case MetaVariables::SERVER_SOFTWARE:
			return metaVariablesMap["SERVER_SOFTWARE="];
	}
	return metaVariablesMap["AUTH_TYPE="];
}

void MetaVariables::setOptionalMetaVars(std::string const &varName,
										std::string const &varValue) {
	optinalMetaVarsMap.insert(std::make_pair(varName + "=", varValue));
}

std::size_t MetaVariables::getSizeMetaVars() const {
	return metaVariablesMap.size() + optinalMetaVarsMap.size();
}

std::map<std::string, std::string> const &MetaVariables::getMetaVariables()
	const {
	return metaVariablesMap;
}

std::map<std::string, std::string> const &
MetaVariables::getOptionalMetaVariables() const {
	return optinalMetaVarsMap;
}

void MetaVariables::init() {
	metaVariablesMap = initMetaVariables();
	optinalMetaVarsMap.clear();
}
