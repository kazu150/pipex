# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/03 16:26:12 by kaisogai          #+#    #+#              #
#    Updated: 2025/08/07 16:30:08 by kaisogai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
PU			= printf_utils
SRCS		= main.c utils.c ft_split.c ft_strjoin.c ft_printf.c $(PU)/ft_putnbr_base.c \
			$(PU)/ft_putnbr_fd.c $(PU)/ft_putuint_fd.c $(PU)/numlen.c $(PU)/print_nums.c \
			$(PU)/print_words.c $(PU)/ft_putchar_fd.c $(PU)/ft_putstr_fd.c
OBJS		= main.o utils.o ft_split.o ft_strjoin.o ft_printf.o $(PU)/ft_putnbr_base.o \
			$(PU)/ft_putnbr_fd.o $(PU)/ft_putuint_fd.o $(PU)/numlen.o $(PU)/print_nums.o \
			$(PU)/print_words.o $(PU)/ft_putchar_fd.o $(PU)/ft_putstr_fd.o

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
