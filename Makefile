NAME = ircserv
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

SRCDIR = .
OBJDIR = obj/
SRC = $(shell find $(SRCDIR) -name '*.cpp')
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)%.o)
LIBS = Parsing/includes/Parsing.hpp *.hpp

RED = "\033[1;31m"
GREEN = "\033[1;32m"
YELLOW = "\033[1;33m"
BLUE = "\033[1;34m"
CYAN = "\033[1;36m"
NC = "\033[0m"

all: $(LIBS) $(NAME)

$(OBJDIR)%.o: $(SRCDIR)/%.cpp $(LIBS)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@echo $(CYAN) "\t>>> Compilation Done <<<\n" $(NC)

clean:
	@rm -rf $(OBJDIR)
	@echo $(YELLOW) "\t-->Clean Done" $(NC)

fclean: clean
	@rm $(NAME)
	@echo $(YELLOW) "\t-->Fclean Done" $(NC)

re: fclean
	@make -s all

.PHONY: all fclean
