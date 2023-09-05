/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 16:59:27 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/05 10:58:43 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "APayload.hpp"

class Request: public virtual APayload
{
	private:
		std::string	url;
		std::string method;
	protected:
	public:
		const std::string	getLines() const;
		bool				setUrl(std::string url);
		std::string const	getUrl(void) const;
		bool				setMethod(std::string method);
		std::string const	getMethod(void) const;
};


