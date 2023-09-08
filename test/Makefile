# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: komatsud <komatsud@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/02 16:27:56 by shtanemu          #+#    #+#              #
#    Updated: 2023/09/08 11:23:35 by komatsud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= google-unit-test
CC				:= c++
CFLAGS			:= -Wall -Wextra -Werror -std=c++14

UNAME_OS		:= $(shell uname -s)

SRC_FILES		:= \
					gtest_main.cpp \
					Response_test.cpp \
					Result_test.cpp \
					Status_test.cpp \
					Config_test.cpp \
					Port_test.cpp

SRC_DIR			:= src
OBJ_DIR			:= obj
SRCS			:= $(SRC_FILES:%.cpp=$(SRC_DIR)/%.cpp)
OBJS			:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

INCLUDES		:= -I./include
LIBS			:= -L./lib

DEPS			:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
DFLAGS			:= -MMD

WS_SRC_FILES	:= \
					APayload.cpp \
					Response.cpp \
					Status.cpp \
					Config.cpp \
					Port.cpp

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

fclean: clean
	@rm -rf $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
