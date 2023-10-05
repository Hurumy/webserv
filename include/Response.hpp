/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:28:25 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/03 19:02:19 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "APayload.hpp"
#include "webserv.hpp"

class Response : public virtual APayload {
   private:
	unsigned int status;
	std::string statusMessage;

   protected:
   public:
	std::string const getLines() const;
	bool setStatus(unsigned int status);
	unsigned int getStatus() const;
	bool setStatusMessage(std::string const &message);
	std::string const &getStatusMessage() const;
};
