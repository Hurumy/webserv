/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:24:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/25 13:13:49 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"
#include "webserv.hpp"

class MethodPost : public AMethod {
	private:
		std::string filename;
		int openPostResource();
		std::string makeFilename(std::string _uppath);
		int writeToFile(int fd);
		Result<int, bool> checkMaxBodySize();

	protected:
	public:
		MethodPost(Config _conf, Request _req, Response &_res);
		~MethodPost();
		Result<int, bool> act();
};
