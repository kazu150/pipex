/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:46 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/09 20:47:42 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	input_child_process(char **args, char *input_file, int d_pipe[2],
		char **envp)
{
	int			fd;
	char		*cmd;
	extern char	**environ;

	close(d_pipe[0]);
	fd = open(input_file, O_RDONLY);
	if (fd == -1)
		(free_split(args), error_exit(input_file));
	cmd = build_command_path(args, envp);
	if (dup2(fd, STDIN_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(fd);
	if (dup2(d_pipe[1], STDOUT_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(d_pipe[1]);
	if (execve(cmd, args, environ) == -1)
		(free(cmd), free_split(args), error_exit(EXECVE));
	return (0);
}
