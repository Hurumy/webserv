/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 18:13:20 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/02 18:17:19 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class	Location
{
	private:
		std::string										uri;
		std::string const 								rootdir;
		unsigned long long								maxbodysize;
		std::map<int, std::string> const				errorpages;
		std::map<std::string, std::string> const		redirects;
		bool											isreturn;
		int												returnstatus;
		std::string										returnurl;
		bool											dirlisting;
		std::vector<std::string> const					index;
		std::string const								uploadpath;
		std::map<std::string, bool> const				reqmethod;
	protected:
	public:
		std::string const								getRootDir() const;
		unsigned long long								getMaxBodySize() const;
		Result<std::string, bool>
};
