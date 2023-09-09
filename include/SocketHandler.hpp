/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 12:12:24 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/09 14:16:07 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <vector>

#include "SSocket.hpp"
#include "CSocket.hpp"

class SocketHandler {
	private:
		SocketHandler();

		std::vector<SSocket> ssockets;
		std::vector<CSocket> csockets;
		std::vector<struct pollfd> pollfds;
	protected:
	public:
		SocketHandler(std::vector<SSocket> &_ssockets);

		std::vector<SSocket> const &getSSockets() const;
		std::vector<CSocket> const &getCSockets() const;
		void addCSocket(CSocket const &_csocket);
		bool createPollfds();
		std::vector<struct pollfd> const &getPollfds() const;
};
