# Variables
NAME = ircserver
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCDIR = .
OBJDIR = obj/
SRC = $(shell find $(SRCDIR) -name '*.cpp')
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)%.o)

# Règles
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(OBJDIR)%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm $(NAME)

.PHONY: all fclean
