/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaVariables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 21:18:00 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/18 21:09:22 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MetaVariables.hpp"

MetaVariables::MetaVariables() : metaVariablesMap(initMetaVariables()) {}

std::map<MetaVariables::tag, std::string> MetaVariables::initMetaVariables() {
	std::map<MetaVariables::tag, std::string> initialMetaVariables;
	
	initialMetaVariables[MetaVariables::AUTH_TYPE] = "";
	initialMetaVariables[MetaVariables::CONTENT_LENGTH] = "";
	initialMetaVariables[MetaVariables::CONTENT_TYPE] = "";
	initialMetaVariables[MetaVariables::GATEWAY_INTERFACE] = "";
	initialMetaVariables[MetaVariables::PATH_TRANSLATED] = "";
	initialMetaVariables[MetaVariables::QUERY_STRING] = "";
	initialMetaVariables[MetaVariables::REMOTE_ADDR] = "";
	initialMetaVariables[MetaVariables::REMOTE_HOST] = "";
	initialMetaVariables[MetaVariables::REMOTE_IDENT] = "";
	initialMetaVariables[MetaVariables::REMOTE_USER] = "";
	initialMetaVariables[MetaVariables::REQUEST_METHOD] = "";
	initialMetaVariables[MetaVariables::SCRIPT_NAME] = "";
	initialMetaVariables[MetaVariables::SERVER_PORT] = "";
	initialMetaVariables[MetaVariables::SERVER_PROTOCOL] = "";
	initialMetaVariables[MetaVariables::SERVER_SOFTWARE] = "";
	return initialMetaVariables;
}

void MetaVariables::setMetaVar(MetaVariables::tag const varName,
							   std::string const &value) {
	metaVariablesMap[varName] = value;
}

std::string const &MetaVariables::getMetaVar(MetaVariables::tag const varName) {
	return metaVariablesMap[varName];
}

void MetaVariables::setOptionalMetaVars(std::string const &varName,
										std::string const &varValue) {
	optinalMetaVarsMap.insert(std::make_pair(varName, varValue));
}

std::size_t MetaVariables::getSizeMetaVars() const {
	return metaVariablesMap.size() + optinalMetaVarsMap.size();
}

std::map<MetaVariables::tag, std::string> const &MetaVariables::getMetaVariables() const {
	return metaVariablesMap;
}

std::map<std::string, std::string> const &MetaVariables::getOptionalMetaVariables() const {
	return optinalMetaVarsMap;
}
