/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:12:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 12:31:16 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "SSocket.hpp"

class SocketHandler {
	private:
		SocketHandler();

		std::vector<SSocket> ssockets;
	protected:
	public:
		SocketHandler(std::vector<SSocket> &_ssockets);

		std::vector<SSocket> const &getSSockets() const;
};
