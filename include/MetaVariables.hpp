/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MetaVariables.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 20:29:31 by shtanemu          #+#    #+#             */
/*   Updated: 2023/10/19 17:16:29 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

#define DEFAULT_METAVARS_SIZE 17L

class MetaVariables {
	public:
		MetaVariables();
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

		void setMetaVar(MetaVariables::tag const varName,
						std::string const &value);
		std::string const &getMetaVar(MetaVariables::tag const varName);
		void setOptionalMetaVars(std::string const &varName,
								 std::string const &varValue);
		std::size_t getSizeMetaVars() const;
		std::map<std::string, std::string> const &getMetaVariables() const;
		std::map<std::string, std::string> const &getOptionalMetaVariables() const;

	protected:
	private:
		static std::map<std::string, std::string> initMetaVariables();
		std::map<std::string, std::string> metaVariablesMap;
		std::map<std::string, std::string> optinalMetaVarsMap;
};
