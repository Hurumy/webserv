/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:04:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 11:03:59 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

class	MethodGet: private AMethod
{
	private:
		std::string			getsemantics;
		Result<int, bool>	setContentType(std::string filename);
		Result<int, bool>	checkGetSemantics();
	protected:
	public:
		MethodGet(Config _conf, Request _req, Response &_res);
		~MethodGet();
		Result<int, bool>	act();
};
