/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:46 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/14 12:39:09 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	input_child_process(char **argv, int d_pipe[2], char **envp)
{
	int			fd;
	char		*cmd;
	extern char	**environ;
	char		**args;
	char		*input_file;

	args = ft_split(argv[2], ' ');
	if (!args)
		error_exit(MALLOC);
	close(d_pipe[0]);
	input_file = argv[1];
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
		execve_error_exit(cmd);
	return (0);
}
