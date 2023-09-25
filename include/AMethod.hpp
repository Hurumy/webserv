/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 10:03:13 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/22 14:00:48 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"

class	AMethod
{
	private:
	protected:
		AMethod(Config _conf, Request _req, Response &_res);
		~AMethod();
		Config								conf;
		Request								req;
		Response							&res;
		Result<std::string, bool>	const	_openFile(std::string filename);
		void								setErrorPageBody();
	public:
		virtual Result<int, bool>			act() = 0;				
};