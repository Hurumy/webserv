/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:09:11 by shtanemu          #+#    #+#             */
/*   Updated: 2023/09/10 12:51:18 by komatsud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <iostream>
# include <iomanip>
# include <map>
# include <algorithm>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <poll.h>
# include <vector>
# include <sstream>
# include <errno.h>

//文字色
# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define GREY "\033[90m"

//背景色
# define RESET_B "\x1b[49m"
# define MAGENTA_B "\x1b[45m"

#define FILE_READ_SIZE 500
#define	DEFAULT_EXT_TYPE "text/plain"
#define	DEFAULT_RETURN_FILE	"./content/readme.html"
#define	DEFAULT_PORT 8080
#define	DEFAULT_LIMIT_CLIENT_BODY_SIZE 500000
#define	DEFAULT_SERVER_NAME	"webserv"


