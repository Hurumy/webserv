/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_unacceptable.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:23:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/06 16:58:54 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "APayload.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Ok.hpp"
#include "Error.hpp"
#include "Result.hpp"

bool	methodUnAcceptable(Request req, Response &res)
{
	std::cout << RED "Unacceptable method: " << req.getMethod() << RESET << std::endl;
	res.setStatus(405);
	res.setStatusMessage("Method Not Allowed");
	return (true);
}

