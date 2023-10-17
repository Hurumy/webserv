/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodGet.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 14:04:56 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/17 12:25:20 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMethod.hpp"

class MethodGet : public AMethod {
   private:
	std::string getsemantics;
	static const std::map<std::string, std::string> ext;
	static std::map<std::string, std::string> initExtMap();
	Result<int, bool> setContentType(std::string filename);
	Result<int, bool> checkGetSemantics();
	Result<int, bool> checkIsDirlisting();

   protected:
   public:
	MethodGet(Config _conf, Request _req, Response &_res);
	~MethodGet();
	Result<int, bool> act();
};
