/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:04:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/20 15:48:21 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "AMethod.hpp"
#include "MakeDirlistHTML.hpp"

class MethodGet : public AMethod {
	private:
		std::string getsemantics;
		static const std::map<std::string, std::string> ext;
		static std::map<std::string, std::string> initExtMap();
		Result<int, bool> setContentType(std::string filename);
		Result<int, bool> checkGetSemantics();
		Result<int, bool> checkIsDirlisting();
		Result<int, bool> searchIndex();

	protected:
	public:
		MethodGet(Config _conf, Request _req, Response &_res);
		~MethodGet();
		Result<int, bool> act();
};
