/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sComp.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 11:14:06 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/05 14:36:37 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class sComp {
	private:
		static unsigned char helper_tolower(unsigned char c);

	protected:
	public:
		bool operator()(std::string const &s1, std::string const &s2) const;
};
