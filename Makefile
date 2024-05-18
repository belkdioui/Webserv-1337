CXX = c++
SRC = main.cpp ./config_file/config_file.cpp ./config_file/partition_server.cpp ./config_file/location_param.cpp
OBJ = $(SRC:.cpp=.o)
NAME = Webserv
RM = rm -rf
CFLAGS = -Wall -Werror -Wextra -std=c++98 #-fsanitize=address -g


all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp %.hpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re