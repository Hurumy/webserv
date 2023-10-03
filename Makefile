# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 14:39:08 by shtanemu          #+#    #+#              #
#    Updated: 2023/10/03 11:51:31 by komatsud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= webserv
CC			:= c++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98 -fsanitize=address

SRC_FILES	:= \
				main.cpp \
				APayload.cpp \
				Response.cpp \
				Request.cpp \
				parseRequest.cpp \
				test.cpp \
				makeStatusMap.cpp \
				Status.cpp \
				Config.cpp \
				Port.cpp \
				CSocket.cpp \
				SSocket.cpp \
				SocketHandler.cpp \
				Address.cpp \
				AMethod.cpp \
				MethodGet.cpp \
				MethodPost.cpp \
				MethodDelete.cpp \
				parseConf.cpp \
				parseConf_putEachLine.cpp \
				parseConf_cutConfToEachPort.cpp \
				parseConf_cutConfByDirective.cpp \
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
				error.cpp \
				stringCleaner.cpp \
				MakeDirlistHTML.cpp \
				Location.cpp \
				

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


all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
