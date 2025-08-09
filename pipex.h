/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:43:44 by vscode            #+#    #+#             */
/*   Updated: 2025/08/09 20:50:21 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define DUP2 "dup2"
# define PIPE "pipe"
# define FORK "fork"
# define ACCESS "access"
# define WAITPID "waitpid"
# define EXECVE "execve"
# define MALLOC "malloc"

int		output(pid_t pid, char **args, char *output_filename, int d_pipe[2],
			char **envp);
int		input_child_process(char **args, char *input_filename, int d_pipe[2],
			char **envp);
int		output_child_process(char **args, char *output_filename, int pipe_in,
			char **envp);
void	error_exit(char *error_target);
char	**ft_split(const char *str, char c);
void	free_split(char **args);
char	*ft_strjoin(const char *str1, const char *str2);
void	handle_command_path_error(char **args, int has_permission_error);
char	*build_command_path(char **args, char **envp);

// ft_printf
int		ft_printf(const char *input, ...);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putuint_fd(unsigned int n, int fd);
void	ft_putnbr_base(unsigned long nbr, char *base);
size_t	ft_strlen(const char *s);
int		print_number(va_list ap);
int		print_unsigned_number(va_list ap);
int		print_any_based_number(va_list ap, char *base, int base_count);
int		print_pointer(va_list ap, char *base);
int		print_string(va_list ap);
int		print_char(va_list ap);
int		any_base_numlen(long n, int base_count);
int		any_base_unsigned_numlen(unsigned long n, int base_count);

#endif
