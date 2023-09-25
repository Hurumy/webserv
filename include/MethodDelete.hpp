/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodDelete.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:15:05 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/25 14:36:41 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "AMethod.hpp"

class	MethodDelete: public AMethod
{
	private:
		int	openResourceDelete();
		int	deleteFile();
	protected:
	public:
		MethodDelete(Config _conf, Request _req, Response &_res);
		~MethodDelete();
		Result<int, bool>	act();
};

