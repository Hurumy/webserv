
#include "APayload.hpp"
#include "Error.hpp"
#include "Ok.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "webserv.hpp"

#define FILE_READ_SIZE 500
#define DEFAULT_EXT_TYPE "text/plain"
#define DEFAULT_RETURN_FILE "./content/readme.html"

std::vector<std::string> lineSpliter(std::string origin, std::string delim);

static unsigned long long setFileSize(std::string Filename, Response &res) {
	unsigned long long bytes = 0;
	int status = 0;
	int fd;
	char buf[FILE_READ_SIZE];
	std::string length;
	std::stringstream ss;

	fd = open(Filename.c_str(), O_RDONLY);
	if (fd == -1) return (0);
	do {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status > 0) bytes += status;
	} while (status > 0);
	if (status == -1) return (0);
	close(fd);
	ss << bytes;
	length = ss.str();
	res.addHeader("Content-Length", length);
	std::cout << "SetFileSize: " << length << std::endl;
	return (bytes);
}

static int setContentType(Response &res, std::string filename) {
	std::map<std::string, std::string> ext;
	std::vector<std::string> list;
	std::string contenttype;

	ext["html"] = "text/html";
	ext["csv"] = "text/csv";
	ext["css"] = "text/css";
	ext["js"] = "text/javascript";
	ext["json"] = "application/json";
	ext["pdf"] = "application/pdf";
	ext["xls"] = "application/vnd.ms-excel";
	ext["xlsx"] =
		"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	ext["ppt"] = "application/vnd.ms-powerpoint";
	ext["pptx"] =
		"application/"
		"vnd.openxmlformats-officedocument.presentationml.presentation";
	ext["doc"] = "application/msword";
	ext["docx"] =
		"application/"
		"vnd.openxmlformats-officedocument.wordprocessingml.document";
	ext["jpg"] = "image/jpeg";
	ext["jpeg"] = "image/jpeg";
	ext["png"] = "image/png";
	ext["gif"] = "image/gif";
	ext["bmp"] = "image/bmp";
	ext["dib"] = "image/bmp";
	ext["svg"] = "image/svg+xml";
	ext["zip"] = "application/zip";
	ext["lzh"] = "application/x-lzh";
	ext["tar"] = "application/x-tar";
	ext["gz"] = "application/x-tar";
	ext["mp3"] = "audio/mpeg";
	ext["mp4"] = "video/mp4";
	ext["mpeg"] = "video/mpeg";
	ext["exe"] = "application/octet-stream";
	ext["out"] = "application/octet-stream";
	ext["txt"] = "text/plain";

	list = lineSpliter(filename, ".");
	if (ext.find(list.back()) != ext.end())
		contenttype = ext.at(list.back());
	else
		contenttype = DEFAULT_EXT_TYPE;
	res.addHeader("Content-Type", contenttype);
	return (0);
}

Result<std::string, int> getFileName(std::string url) {
	if (url.empty() == true) return Error<int>(false);
	if (url == "/")
		return Ok<std::string>(DEFAULT_RETURN_FILE);
	else
		return Ok<std::string>(url);
}

Result<bool, int> readFileContents(Response &res, std::string filename) {
	int fd;
	int status = 1;
	std::string body;
	char buf[FILE_READ_SIZE];

	// open
	fd = open(filename.c_str(), O_RDONLY);
	if (fd == -1 && errno == ENOENT) {
		res.setStatus(404);
		res.setStatusMessage("Not Found");
		return Error<int>(-1);
	} else if (fd == -1 && errno == EACCES) {
		res.setStatus(403);
		res.setStatusMessage("Forbidden");
		return Error<int>(-1);
	}
	// read
	while (status > 0) {
		status = read(fd, buf, FILE_READ_SIZE);
		if (status != -1) body += buf;
	}
	close(fd);
	// set
	if (status == -1) {
		res.setStatus(500);
		res.setStatusMessage("Internal Server Error");
		return Error<int>(-1);
	}
	// std::cout << "Body: " << body << std::endl;
	res.setBody(body);
	return Ok<bool>(true);
}

int makeResBody(Request req, Response &res) {
	std::string filename;
	std::string body;

	//ファイルの名前が正しいかどうかを見る
	//ルートが指定された時は適切なパスを返す
	Result<std::string, int> result = getFileName(req.getUrl());
	if (result.isOK() == true) filename = result.getOk();
	//拡張子を見てContentTypeを判断しResponseにセット
	setContentType(res, filename);
	//ファイルサイズを数えResponseにセット ついでにOpenが可能かどうかも見る
	setFileSize(filename, res);
	//ファイルの中身を読み込んでBodyに詰める
	Result<bool, int> res_read = readFileContents(res, filename);
	if (res_read.isError() == true) return (-1);
	return (0);
}
