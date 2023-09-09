/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CSocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 13:01:41 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 13:15:44 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CSocket.hpp"

CSocket::CSocket(int const _sockfd) : sockfd(_sockfd), revents(0) {}

int CSocket::getSockfd() const {
	return sockfd;
}

short CSocket::getRevents() const {
	return revents;
}

void CSocket::setRevents(short const _revents) {
	revents = _revents;
}
