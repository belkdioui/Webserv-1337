# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/22 10:15:25 by rarraji           #+#    #+#              #
#    Updated: 2024/06/04 09:59:21 by rarraji          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = webserv

CC      = c++
FLAGS   = -Wall -Wextra -Werror -std=c++98 
RM      = rm -rf
SRC     = main.cpp Server.cpp request.cpp cgi.cpp response.cpp infoserv.cpp ./config_file/config_file.cpp ./config_file/partition_server.cpp ./config_file/location_param.cpp ./config_file/utils.cpp
OBJ     = $(SRC:.cpp=.o) 

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
3YELLOW = \033[0;36m

%.o : %.cpp Server.hpp
	@printf "✅ ---> "
	${CC} ${FLAGS} -c $< -o $@

all: $(NAME)
	@printf "	      _                        \n" 
	@printf "__      _____| |__  ___  ___ _ ____   __\n"
	@printf "\ \ /\ / / _ \ '_ \/ __|/ _ \ '__\ \ / /\n"
	@printf " \ V  V /  __/ |_) \__ \  __/ |   \ V / \n"
	@printf "  \_/\_/ \___|_.__/|___/\___|_|    \_/  \n"
	@printf "           $(3YELLOW)Solix like Nginx\n"
	@printf "\n"
	@printf "\n"

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

re: fclean all

.PHONY: all clean fclean re

run :
	./Server config_file/config_file.yaml