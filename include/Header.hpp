/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 10:54:35 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/04 11:29:53 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "APayload.hpp"
#include "sComp.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"


class Header
{
    private:
        std::map<std::string, std::string, sComp>  header;
    protected:
    public:
        bool addHeader(std::string const &key, std::string const &value);
		bool setHeader(std::string const &key, std::string const &value);
		Result<std::string, bool> const getHeader(std::string const &key) const;
		std::map<std::string, std::string, sComp> const &getAllHeader() const;
};


