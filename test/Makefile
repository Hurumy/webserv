# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shtanemu <shtanemu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 16:27:56 by shtanemu          #+#    #+#              #
#    Updated: 2023/11/07 12:26:30 by shtanemu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= google-unit-test
CC				:= c++
CFLAGS			:= -Wall -Wextra -Werror -std=c++14

UNAME_OS		:= $(shell uname -s)
ifeq ($(UNAME_OS), Linux)
	CFLAGS	+= -D_LINUX
else ifeq ($(UNAME_OS), Darwin)
	CFLAGS	+= -D_DARWIN
endif

SRC_FILES		:= \
					gtest_main.cpp \
					Response_test.cpp \
					Result_test.cpp \
					Status_test.cpp \
					Config_test.cpp \
					Port_test.cpp \
					ConfParser_test.cpp \
					Address_test.cpp \
					SSocket_test.cpp \
					CSocket_test.cpp \
					SocketHandler_test.cpp \
					Request_test.cpp \
					RequestHandler_test.cpp \
					MethodGet_test.cpp \
					MethodPost_test.cpp \
					MethodDelete_test.cpp \
					MakeDirListHTML_test.cpp \
					Location_test.cpp \
					LocationParser_test.cpp \

SRC_DIR			:= src
OBJ_DIR			:= obj
SRCS			:= $(SRC_FILES:%.cpp=$(SRC_DIR)/%.cpp)
OBJS			:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

INCLUDES		:= -I./include
LIBS			:= -L./lib

DEPS			:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
DFLAGS			:= -MMD

WS_SRC_FILES	:= \
					Server.cpp \
					APayload.cpp \
					Response.cpp \
					Request.cpp \
					Version.cpp \
					test.cpp \
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
					ft.cpp

WS_SRC_DIR		:= ../src
WS_OBJ_DIR		:= wsobj
WS_SRCS			:= $(WS_SRC_FILES:%.cpp=$(WS_SRC_DIR)/%.cpp)
WS_OBJS			:= $(patsubst $(WS_SRC_DIR)/%.cpp, $(WS_OBJ_DIR)/%.o, $(WS_SRCS))

WS_INCLUDES		:= -I../include

WS_DEPS			:= $(patsubst $(WS_SRC_DIR)/%.cpp, $(WS_OBJ_DIR)/%.d, $(WS_SRCS))
WS_DFLAGS		:= -MMD


$(NAME): $(OBJS) $(WS_OBJS) $(DEPS)
	$(CC) $(INCLUDES) $(WS_INCLUDES) -o $(NAME) $(OBJS) $(WS_OBJS) $(LIBS) -lgtest -lgtest_main -lpthread $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(INCLUDES) $(WS_INCLUDES) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(WS_OBJ_DIR)/%.o: $(WS_SRC_DIR)/%.cpp
	@mkdir -p $(WS_OBJ_DIR)
	$(CC) $(WS_INCLUDES) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(DEPS):

$(WS_DEPS):


utest: re
	./$(NAME)

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(WS_OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
