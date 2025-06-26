# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vscode <vscode@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/03 16:26:12 by kaisogai          #+#    #+#              #
#    Updated: 2025/06/26 18:41:21 by vscode           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
SRC_DIR = src
OBJ_DIR = obj
TMP_DIR = tmp_objs

SRCS = main.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(TMP_DIR)
	cp $(LIBFT) $(TMP_DIR)/
	cd $(TMP_DIR) && ar x libft.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(addprefix $(TMP_DIR)/, $(LIBFT_OBJS))
	rm -rf $(TMP_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

debug: CFLAGS += -g

re: fclean all

.PHONY: all clean fclean re
