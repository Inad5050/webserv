NAME				:= webserv
CXX					:= c++

CXXFLAGS			:= -Wall -Wextra -Werror -std=c++98

SRC_DIR				:= src
OBJ_DIR				:= obj
INC_DIR				:= include

GREEN				= \033[0;92m
CLEAR				= \033[0m

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: release

release: $(NAME)
	@echo "$(GREEN)[✅ Release build]$(CLEAR) $(NAME)"

debug: CXXFLAGS := $(CXXFLAGS) -DDEBUG
debug: $(NAME)
	@echo "$(GREEN)[🐞 Debug build]$(CLEAR) $(NAME)"

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)[🧹 Cleaned object files]$(CLEAR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)[🧹 Cleaned executable]$(CLEAR)"

re: fclean all

.PHONY: all debug release clean fclean re deleteme
