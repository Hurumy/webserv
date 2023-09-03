
CC = c++

CXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = *.cpp \
	   payload_classes/*.cpp \
	   statuscode/*.cpp

NAME = webserv

RM = rm -rf

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CXFLAGS) $(SRCS) -o $(NAME)

.PHONY: clean fclean re all

clean:

fclean: clean
	$(RM) $(NAME)

re: fclean all


