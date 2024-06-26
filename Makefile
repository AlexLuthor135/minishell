# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/08 16:46:30 by alappas           #+#    #+#              #
#    Updated: 2024/05/15 20:10:44 by alappas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
CFLAGS				=	-Wall -Wextra -Iinclude -Isrc -O3 -g #-fsanitize=address

RM					=	rm -rf

NAME				=	minishell

# readline, leaks, valgrind
RL_PREFIX = $(HOME)/.local/pkg/readline
RL_CFLAGS = -I $(RL_PREFIX)/include
RL_LIBS   = -L $(RL_PREFIX)/lib -lreadline -lhistory -lcurses
VALGRIND  = valgrind --track-fds=yes --leak-check=full #--show-leak-kinds=all -s --track-origins=yes
LEAKS	  = leaks --atExit --

# Libraries
LIBFT				=	libft.a
LIBFT_DIR			=	libft
LIBFT_FILE			=	$(LIBFT_DIR)/$(LIBFT)
CFLAGS				+=	-Wall \
						-Wextra \
						-I $(LIBFT_DIR)/include

MAKE_LIB			=	make --no-print-directory -C

# Source and Object Files
VPATH				=	src:include:src/execution:src/lexer:src/parsing:src/quotes:src/utils:include
MINISHELL_INC		=	minishell.h
MINISHELL_SRC		=	$(shell find src -name '*.c')

# Object Files
OBJ_DIR				=	obj
MINISHELL_OBJ		=	$(addprefix $(OBJ_DIR)/, $(MINISHELL_SRC:.c=.o))

# Rules
all:				$(NAME)

$(OBJ_DIR)/%.o:		%.c $(MINISHELL_INC)
					@mkdir -p $(dir $@)
					@cc $(CFLAGS) $(RL_CFLAGS) -c $< -o $@

$(LIBFT_FILE):
					$(MAKE_LIB) $(LIBFT_DIR)

$(NAME):			$(MINISHELL_OBJ)
					@make -C $(LIBFT_DIR)
					@cc $(CFLAGS) $(MINISHELL_OBJ) $(LIBFT_FILE) $(RL_LIBS) -o $@
					@echo "$(GREEN)$(NAME) was successfully created!$(DEFAULT)"

lib_clean:
					$(MAKE_LIB) $(LIBFT_DIR) clean

lib_fclean:
					$(MAKE_LIB) $(LIBFT_DIR) fclean

clean:				lib_clean
					@$(RM) $(OBJ_DIR)
					@echo "$(YELLOW)$(NAME) object files deleted!$(DEFAULT)"

fclean:				clean lib_fclean
					@$(RM) $(NAME)
					@echo "$(RED)$(NAME) program deleted!$(DEFAULT)"
					
re:					fclean all

# Valgrind testing
valgrind: $(NAME)
	$(VALGRIND) ./$(NAME)

# Leaks at exit testing
leaks: $(NAME)
	$(LEAKS) ./$(NAME)

#.SILENT:

.PHONY:				all lib_clean lib_fclean clean fclean re

# COLORS DEFINITION
RED				= \033[1;31m
DEFAULT			= \033[0m
GREEN			= \033[1;32m
BOLD			= \033[1m
YELLOW			= \033[1;33m