/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodPost.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 10:24:25 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/16 15:33:42 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"
#include "webserv.hpp"

class MethodPost : public AMethod {
	private:
		std::string filename;
		int openPostResource();
		std::string makeFilename(std::string _uppath);
		Result<int, bool> checkMaxBodySize();
		static const std::map<std::string, std::string> ext;
		static	std::map<std::string, std::string> initExtMap();
		Result<std::string, bool> setExtension(std::string fname, std::string type) const;

	protected:
	public:
		MethodPost(Config _conf, Request _req, Response &_res);
		~MethodPost();
		Result<int, bool> act();
};
