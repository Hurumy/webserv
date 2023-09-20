/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iR_accept.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:11:30 by komatsud          #+#    #+#             */
/*   Updated: 2023/09/20 14:30:19 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initResponse.hpp"

static std::vector<std::string>	makeContentTypeVector()
{
	std::vector<std::string> vec;

	vec.push_back("*/*");
	vec.push_back("application/*");
	vec.push_back("application/EDI-X12");
	vec.push_back("application/EDIFACT");
	vec.push_back("application/javascript");
	vec.push_back("application/octet-stream");
	vec.push_back("application/ogg");
	vec.push_back("application/pdf");
	vec.push_back("application/xhtml+xml");
	vec.push_back("application/x-shockwave-flash");
	vec.push_back("application/json");
	vec.push_back("application/ld+json");
	vec.push_back("application/xml");
	vec.push_back("application/zip");
	vec.push_back("application/x-www-form-urlencoded");
	vec.push_back("audio/*");
	vec.push_back("audio/mpeg");
	vec.push_back("audio/x-ms-wma");
	vec.push_back("audio/vnd.rn-realaudio");
	vec.push_back("audio/x-wav");
	vec.push_back("image/*");
	vec.push_back("image/gif");
	vec.push_back("image/jpeg");
	vec.push_back("image/png");
	vec.push_back("image/tiff");
	vec.push_back("image/vnd.microsoft.icon");
	vec.push_back("image/x-icon");
	vec.push_back("image/vnd.djvu");
	vec.push_back("image/svg+xml");
	vec.push_back("multipart/*");
	vec.push_back("multipart/mixed");
	vec.push_back("multipart/alternative");
	vec.push_back("multipart/related");
	vec.push_back("multipart/form-data");
	vec.push_back("text/*");
	vec.push_back("text/css");
	vec.push_back("text/csv");
	vec.push_back("text/html");
	vec.push_back("text/javascript");
	vec.push_back("text/plain");
	vec.push_back("text/xml");
	vec.push_back("video/*");
	vec.push_back("video/mpeg");
	vec.push_back("video/mp4");
	vec.push_back("video/quicktime");
	vec.push_back("video/x-ms-wmv");
	vec.push_back("video/x-msvideo");
	vec.push_back("video/x-flv");
	vec.push_back("video/webm");
	vec.push_back("application/vnd.oasis.opendocument.text");
	vec.push_back("application/vnd.oasis.opendocument.spreadsheet");
	vec.push_back("application/vnd.oasis.opendocument.presentation");
	vec.push_back("application/vnd.oasis.opendocument.graphics");
	vec.push_back("application/vnd.ms-excel");
	vec.push_back("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
	vec.push_back("application/vnd.ms-powerpoint");
	vec.push_back("application/vnd.openxmlformats-officedocument.presentationml.presentation");
	vec.push_back("application/msword");
	vec.push_back("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	vec.push_back("application/vnd.mozilla.xul+xml");
	
	return (vec);
}

static	std::map<>

int	iR_accept(Request req, Response &res, Config conf)
{
	
}
