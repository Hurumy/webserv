/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:26:40 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 12:32:19 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketHandler.hpp"

#include <vector>
#include "SSocket.hpp"

SocketHandler::SocketHandler(std::vector<SSocket> &_ssockets) : ssockets(_ssockets) {}

std::vector<SSocket> const &SocketHandler::getSSockets() const {
	return ssockets;
}
