NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g
LIBS = Irc.hpp Server.hpp Client.hpp 

SRCS = main.cpp Server.cpp Irc.cpp Client.cpp commands.cpp Modes.cpp
OBJS_DIR = obj
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

RED = "\033[1;31m"
GREEN = "\033[1;32m"
YELLOW = "\033[1;33m"
BLUE = "\033[1;34m"
CYAN = "\033[1;36m"
NC = "\033[0m"

all : $(LIBS) $(NAME)

$(OBJS_DIR)/%.o:%.cpp
	@mkdir -p obj
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJS) Makefile
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo $(CYAN) "Compilation Done\n" $(NC)

clean :
	@rm -rf $(OBJS) $(OBJS_DIR)
	@echo $(YELLOW) "Clean Done\n" $(NC)

fclean : clean
	@rm $(NAME)
	@echo $(YELLOW) "FClean Done\n" $(NC)

re : fclean all