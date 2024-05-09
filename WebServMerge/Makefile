# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/22 10:15:25 by rarraji           #+#    #+#              #
#    Updated: 2024/05/08 15:47:21 by rarraji          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = Server

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address -g
RM      = rm -rf
SRC     = main.cpp Server.cpp request.cpp cgi.cpp response.cpp infoserv.cpp location.cpp ParseServer.cpp ./config_file/config_file.cpp ./config_file/partition_server.cpp ./config_file/location_param.cpp
OBJ     = $(SRC:.cpp=.o) 

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m

%.o : %.cpp Server.hpp
	${CC} ${FLAGS} -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(YELLOW) Compiling $(NAME)... \n"
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN) Executable ready.\n"

clean:
	@$(RM) $(OBJ)
	@printf "$(RED)    - OBJ removed.\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(RED)    - Executable removed.\n"

re: all clean

.PHONY: all clean fclean re