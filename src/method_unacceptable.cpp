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

#include "APayload.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

bool methodUnAcceptable(Request req, Response &res) {
	std::cout << RED "Unacceptable method: " << req.getMethod() << RESET
			  << std::endl;
	res.setStatus(405);
	res.setStatusMessage("Method Not Allowed");
	return (true);
}
