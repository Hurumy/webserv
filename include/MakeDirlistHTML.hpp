/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeDirlistHTML.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:42:15 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/02 14:52:37 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Result.hpp"
#include "Ok.hpp"
#include "Error.hpp"
# include <dirent.h>
# include <string.h>
# include <time.h>
# include <ctime>

class MakeDirlistHTML
{
	private:
		std::string					path;
		std::string					html;
		static const std::string	header;
		static const size_t			bufsize;
	protected:
	public:
		MakeDirlistHTML(std::string _path);
		~MakeDirlistHTML();
		Result<std::string, bool>	returnHTML();
};



