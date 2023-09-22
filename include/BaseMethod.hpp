/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseMethod.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 10:03:13 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/22 13:53:21 by komatsud         ###   ########.fr       */
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

class	BaseMethod
{
	private:
	protected:
		BaseMethod(Config _conf, Request _req, Response &_res);
		~BaseMethod();
		Config								conf;
		Request								req;
		Response							&res;
		Result<std::string, bool>	const	_openFile(std::string filename);
		void								setErrorPageBody();
	public:
};
