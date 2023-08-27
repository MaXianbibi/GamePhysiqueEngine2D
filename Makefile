NAME = WatanabeEngine
CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -O2
RM = rm -f

SRC_DIR = srcs/
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJ_DIR = objs/
OBJS = $(SRC_FILES:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) 
	@$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $@
	@echo "\r\033[33mDONE!\033[0m         "

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

NES: 
	$(MAKE) SYS=1

.PHONY: all clean fclean re NES
