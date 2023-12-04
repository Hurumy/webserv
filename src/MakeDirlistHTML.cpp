/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeDirlistHTML.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:46:54 by komatsud          #+#    #+#             */
/*   Updated: 2023/11/15 10:22:56 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MakeDirlistHTML.hpp"
#if defined(__APPLE__) || defined(__NetBSD__)
#define st_atim st_atimespec
#define st_ctim st_ctimespec
#define st_mtim st_mtimespec
#endif

const std::string MakeDirlistHTML::header = "<html>\n";
const size_t MakeDirlistHTML::bufsize = 200;

MakeDirlistHTML::MakeDirlistHTML(std::string _path) : path(_path) {}

MakeDirlistHTML::~MakeDirlistHTML() {}

Result<std::string, bool> MakeDirlistHTML::returnHTML() {
	html += header;
	html += "\t<head><title>Index of ";
	html += path;
	html += "</title><head>\n";
	html += "\t<body>\n";
	html += "\t\tIndex of ";
	html += path;
	html += "</h1><hr><pre><a href=\"../\">../</a>\n";

	//ディレクトリを開く
	DIR *ds = opendir(path.c_str());
	if (ds == NULL) return Error<bool>(false);

	//ディレクトリにあるものの要素を読んでいく
	errno = 0;
	struct dirent *st = readdir(ds);
	if (st == NULL && errno != 0) {
		closedir(ds);
		return Error<bool>(false);
	}

	// HTMLに詰める
	struct stat sstat;
	int status;
	std::string tmpfilepath;
	struct timespec stime;
	char buf[bufsize];
	off_t filesize;
	std::string tmpfs;
	std::stringstream ss;

	while (st != NULL) {
		//ファイル名を格納
		html += "<a href=\"";
		html += st->d_name;
		html += "\">";
		html += st->d_name;
		html += "</a>\t\t\t\t";

		// statでファイルの作成日時を取得する
		tmpfilepath = path + st->d_name;
		status = stat(tmpfilepath.c_str(), &sstat);
		if (status == -1) {
			closedir(ds);
			return Error<bool>(false);
		}

		//上のDefineで差異を舐めしてくれてありがとう！！@shtanemu
		stime = sstat.st_mtim;

		strftime(buf, sizeof(buf), "%d-%b-%Y %R", gmtime(&stime.tv_sec));
		html += buf;
		bzero(buf, bufsize);

		html += "\t\t\t\t";

		//ファイルサイズを書き込む
		filesize = sstat.st_size;
		ss << filesize;
		ss >> tmpfs;
		html += tmpfs;

		html += "\n";

		//もう一度読む
		st = readdir(ds);
	}
	if (errno != 0) {
		closedir(ds);
		return Error<bool>(false);
	}

	closedir(ds);

	//終わりの部分を書き込む
	html += "</pre><hr></body>\n";
	html += "</html>\n";

	// std::cout << YELLOW << html << RESET << std::endl;

	return Ok<std::string>(html);
}
