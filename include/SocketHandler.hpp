/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:12:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 13:33:31 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "SSocket.hpp"
#include "CSocket.hpp"

class SocketHandler {
	private:
		SocketHandler();

		std::vector<SSocket> ssockets;
		std::vector<CSocket> csockets;
	protected:
	public:
		SocketHandler(std::vector<SSocket> &_ssockets);

		std::vector<SSocket> const &getSSockets() const;
		std::vector<CSocket> const &getCSockets() const;
		void addCSocket(CSocket const &_csocket);
};
