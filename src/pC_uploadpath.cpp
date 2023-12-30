/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pC_uploadpath.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:51:35 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/07 12:20:31 by shtanemu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int readUploadPath(Config &conf, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "upload_path")
		ft::errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		ft::errorInInit("Too many elements in upload_path directive");

	if (conf.getUploadPath().empty() == false)
		ft::errorInInit("Too many upload_path declare");

	// パスが有効かチェック
	DIR	*_stat = opendir(lines.at(1).c_str());
	if (_stat == NULL)
	{
		std::string	errm = "upload_path: ";
		if (errno == ENOTDIR)
		{
			errm += "Not a Directory, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENAMETOOLONG)
		{
			errm += "Path is too long, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENOENT)
		{
			errm += "Can't find the location, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ELOOP)
		{
			errm += "Loop detected, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == EMFILE)
		{
			errm += "Can't get a file descriptor, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENFILE)
		{
			errm += "file table overflow, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == EACCES)
		{
			errm += "It is forbidden, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENOMEM)
		{
			errm += "Memory is not enough, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
	}
	closedir(_stat);

	conf.setUploadPath(lines.at(1));

	return (0);
}

int l_readUploadPath(Location &loc, std::string oneline) {
	std::vector<std::string> lines;

	lines = lineSpliter(oneline, " ");

	lines.erase(std::remove(lines.begin(), lines.end(), ""), lines.end());

	if (lines.at(0) != "upload_path")
		ft::errorInInit("Unknown directive detected!(ﾉｼ｀･ω･)ﾉｼ");

	if (lines.size() != 2)
		ft::errorInInit("Too many elements in upload_path directive");

	if (loc.getUploadPath().empty() == false)
		ft::errorInInit("Too many upload_path declare");

	// パスが有効かチェック
	DIR	*_stat = opendir(lines.at(1).c_str());
	if (_stat == NULL)
	{
		std::string	errm = "upload_path: ";
		if (errno == ENOTDIR)
		{
			errm += "Not a Directory, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENAMETOOLONG)
		{
			errm += "Path is too long, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENOENT)
		{
			errm += "Can't find the location, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ELOOP)
		{
			errm += "Loop detected, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == EMFILE)
		{
			errm += "Can't get a file descriptor, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENFILE)
		{
			errm += "file table overflow, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == EACCES)
		{
			errm += "It is forbidden, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
		else if (errno == ENOMEM)
		{
			errm += "Memory is not enough, in ";
			errm += lines.at(1);
			ft::errorInInit(errm);
		}
	}
	closedir(_stat);

	loc.setUploadPath(lines.at(1));

	return (0);
}
