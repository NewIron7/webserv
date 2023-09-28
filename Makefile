# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/10 16:09:31 by hboissel          #+#    #+#              #
#    Updated: 2023/09/28 13:28:40 by hboissel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv

CC            = c++

CFLAGS        = -g3 -Wall -Wextra -Werror -std=c++98 -pedantic-errors

# Directories
SRCS_DIR    = src/
OBJS_DIR    = bin/
INCS_DIR    = inc/
DEPS_DIR	= bin/

# Source files
SRCS_FILES    = main.cpp TcpServer.cpp Sockets.cpp InternalError.cpp
OBJS_FILES    = $(SRCS_FILES:.cpp=.o)
DEPS_FILES    = $(SRCS_FILES:.cpp=.d)

# Paths
SRCS        = $(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS        = $(addprefix $(OBJS_DIR), $(OBJS_FILES))
DEPS        = $(addprefix $(DEPS_DIR), $(DEPS_FILES))

# Colors / Actions
RESET        = \033[0m
GREEN        = \033[32m
YELLOW        = \033[33m
BLUE        = \033[34m
RED            = \033[31m
UP            = \033[A
CUT            = \033[K

# Main rule
all:        $(NAME)

# Executable compilation rule
$(NAME):    $(OBJS)
	@echo "$(YELLOW)Compiling executable file [$(NAME)]...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[$(NAME)] is compiled!$(RESET)"

# Object files compilation rule
$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(OBJS_DIR)
	@echo "$(YELLOW)Compiling [$@]...$(RESET)"
	$(CC) $(CFLAGS) -I $(INCS_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished compiling [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

# Other rules
clean:
	@if [ -d "$(OBJS_DIR)" ]; then \
	echo "$(BLUE)Deleting object files...$(RESET)"; \
	rm -rf $(OBJS_DIR); \
	echo "$(GREEN)Object files deleted!$(RESET)"; else \
	echo "No object files to delete."; \
	fi;

fclean:        clean
	@if [ -f "$(NAME)" ]; then \
	echo "$(BLUE)Deleting executable file...$(RESET)"; \
	rm -rf $(NAME); \
	echo "$(GREEN)Executable file deleted!$(RESET)"; else \
	echo "No executable file to delete."; \
	fi;

re:            fclean all

-include $(DEPS)

.PHONY:        all clean fclean re
