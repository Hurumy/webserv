/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeDirlistHTML.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:42:15 by komatsud          #+#    #+#             */
/*   Updated: 2023/10/17 15:37:25 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <string.h>
#include <time.h>

#include <ctime>

#include "Error.hpp"
#include "Ok.hpp"
#include "Result.hpp"
#include "webserv.hpp"

class MakeDirlistHTML {
	private:
		std::string path;
		std::string	uri_without_root;
		std::string html;
		static const std::string header;
		static const size_t bufsize;

	protected:
	public:
		MakeDirlistHTML(std::string _path, std::string _uri_without_root);
		~MakeDirlistHTML();
		Result<std::string, bool> returnHTML();
};
