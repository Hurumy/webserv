/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:24:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/23 12:39:35 by komatsud         ###   ########.fr       */
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
