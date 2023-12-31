# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:39:08 by shtanemu          #+#    #+#              #
#    Updated: 2023/12/25 11:05:37 by shtanemu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= webserv
CC			:= c++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98
UNAME_OS	:= $(shell uname -s)
ifeq ($(UNAME_OS), Linux)
	DBFLAGS	:= -Wshadow -fsanitize=address -g -D_DEBUGFLAG
	CFLAGS	+= -D_LINUX
else ifeq ($(UNAME_OS), Darwin)
	DBFLAGS	:= -Wshadow-all -fsanitize=address -g -D_DEBUGFLAG
	CFLAGS	+= -D_DARWIN
endif
ifeq ($(MAKECMDGOALS), debug)
	CFLAGS += $(DBFLAGS)
endif

SRC_FILES	:= \
				main.cpp \
				Server.cpp \
				APayload.cpp \
				Response.cpp \
				Request.cpp \
				Version.cpp \
				makeStatusMap.cpp \
				Status.cpp \
				Config.cpp \
				Port.cpp \
				CSocket.cpp \
				SSocket.cpp \
				SocketHandler.cpp \
				CGIResponseCreator.cpp \
				MetaVariables.cpp \
				Address.cpp \
				AMethod.cpp \
				MethodGet.cpp \
				MethodPost.cpp \
				MethodDelete.cpp \
				parseRequest.cpp \
				parseConf.cpp \
				parseConf_cutConfByDirective.cpp \
				parseConf_cutConfToEachPort.cpp \
				parseConf_putEachLine.cpp \
				pC_listen.cpp \
				pC_return.cpp \
				pC_root.cpp \
				pC_rewrite.cpp \
				pC_servername.cpp \
				pC_autoindex.cpp \
				pC_errorpage.cpp \
				pC_maxbodysize.cpp \
				pC_index.cpp \
				pC_uploadpath.cpp \
				pC_allowedmethods.cpp \
				RequestHandler.cpp \
				stringCleaner.cpp \
				MakeDirlistHTML.cpp \
				Location.cpp \
				parseConf_parseLocation.cpp \
				pC_location.cpp \
				pC_cgi_path.cpp \
				pC_alias.cpp \
				sComp.cpp \
				ft.cpp \
				AddrMonitor.cpp

SRC_DIR		:= src
OBJ_DIR		:= obj

SRCS		:= $(SRC_FILES:%.cpp=$(SRC_DIR)/%.cpp)
OBJS		:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o,$(SRCS))

INCLUDES	:= -I./include

DEPS		:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
DFLAGS		:= -MMD


$(NAME): $(OBJS) $(DEPS)
	$(CC) $(INCLUDES) -o $(NAME) $(OBJS) $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(INCLUDES) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(DEPS):


debug: all

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
