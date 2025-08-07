/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:43:44 by vscode            #+#    #+#             */
/*   Updated: 2025/08/07 15:48:13 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
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

int		input_parent_process(pid_t pid, char **args, char *output_filename,
			int d_pipe[2]);
int		input_child_process(char **args, char *input_filename, int d_pipe[2]);
int		output_child_process(char **args, char *output_filename, int pipe_in);
void	error_exit(char *error_target);
char	**ft_split(const char *str, char c);
void	free_split(char **args);
char	*ft_strjoin(const char *str1, const char *str2);

#endif
