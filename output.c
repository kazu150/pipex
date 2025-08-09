/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:18:32 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/09 15:18:37 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	output_child_process(char **args, char *output_filename, int pipe_in)
{
	int		fd;
	char	*cmd;

	cmd = build_command_path(args);
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(pipe_in);
	fd = open(output_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		(free(cmd), free_split(args), error_exit(output_filename));
	if (dup2(fd, STDOUT_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(fd);
	if (execve(cmd, args, 0) == -1)
		(free(cmd), free_split(args), error_exit(EXECVE));
	free_split(args);
	return (0);
}

int	output(pid_t pid, char **args, char *output_filename, int d_pipe[2])
{
	int		status;
	int		o_status;
	pid_t	o_pid;

	close(d_pipe[1]);
	o_pid = fork();
	if (o_pid < 0)
		(free_split(args), error_exit(FORK));
	if (o_pid == 0)
		return (output_child_process(args, output_filename, d_pipe[0]));
	else
	{
		free_split(args);
		if (waitpid(pid, &status, 0) < 0 || waitpid(o_pid, &o_status, 0) < 0)
			(free_split(args), error_exit(WAITPID));
		if (WIFEXITED(status) && WIFEXITED(o_status))
		{
			close(d_pipe[0]);
			return (WEXITSTATUS(o_status));
		}
	}
	exit(0);
}
