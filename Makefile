NAME				:= webserv
CXX					:= c++

CXXFLAGS_release	:= -Wall -Wextra -Werror -std=c++98
CXXFLAGS_debug		:=  $(CXXFLAGS_release) -DNDEBUG

SRC_DIR				:= src
OBJ_DIR				:= obj
INC_DIR				:= include

DEF_COLOR			= \033[0;39m
GRAY				= \033[0;90m
RED					= \033[0;31m
GREEN				= \033[0;92m
YELLOW				= \033[0;93m
BLUE				= \033[0;94m
MAGENTA				= \033[0;95m
CYAN				= \033[0;96m
WHITE				= \033[0;97m
CLEAR				= \033[0m

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: release

release: CXXFLAGS := $(CXXFLAGS_release)
release: $(NAME)
	@echo "$(GREEN)[✅ Release build]$(CLEAR) $(NAME)"

debug: CXXFLAGS := $(CXXFLAGS_debug)
debug: $(NAME)
	@echo "$(CYAN)[🐞 Debug build]$(CLEAR) $(NAME)"

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)[🧹 Cleaned object files]$(CLEAR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)[🧹 Cleaned executable]$(CLEAR)"

re: fclean all

.PHONY: all debug release clean fclean re deleteme
