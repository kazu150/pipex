/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:43:44 by vscode            #+#    #+#             */
/*   Updated: 2025/06/29 16:31:13 by kaisogai         ###   ########.fr       */
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
# define WAITPID "waitpid"
# define EXECVE "execve"

int		input_parent_process(pid_t pid, char **args, char **argv,
			int d_pipe[2]);
int		input_child_process(char **args, char **argv, int d_pipe[2]);
int		output_child_process(char **args, char *output_command,
			char *output_filename, int pipe_in);
void	error_exit(char *error_target);
void	build_args(char *args[4], char *input_command);

#endif
