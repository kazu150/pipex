/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/27 18:21:56 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	build_args(char *args[4], char *input_command)
{
	args[0] = "sh";
	args[1] = "-c";
	args[2] = input_command;
	args[3] = NULL;
}

void	error_exit(char *error_target)
{
	perror(error_target);
	exit(EXIT_FAILURE);
}

int	output_child_process(char **args, char *output_command,
		char *output_filename, int pipe_in)
{
	int	fd;

	args[2] = output_command;
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		error_exit("dup2");
	close(pipe_in);
	fd = open(output_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		perror(output_filename);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(fd);
	if (execve("/bin/sh", args, 0) == -1)
		exit(EXIT_FAILURE);
	return (0);
}

int	input_child_process(char **args, int d_pipe[2])
{
	close(d_pipe[0]);
	if (dup2(d_pipe[1], STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(d_pipe[1]);
	if (execve("/bin/sh", args, 0) == -1)
		exit(EXIT_FAILURE);
	return (0);
}

int	input_parent_process(pid_t pid, char **args, char **argv, int d_pipe[2])
{
	int		status;
	int		o_status;
	pid_t	o_pid;

	close(d_pipe[1]);
	o_pid = fork();
	if (o_pid < 0)
		exit(EXIT_FAILURE);
	if (o_pid == 0)
		return (output_child_process(args, argv[3], argv[4], d_pipe[0]));
	else
	{
		if (waitpid(pid, &status, 0) < 0 || waitpid(o_pid, &o_status, 0) < 0)
			exit(EXIT_FAILURE);
		if (WIFEXITED(status) && WIFEXITED(o_status))
		{
			close(d_pipe[0]);
			return (0);
		}
	}
	return (0);
}

//  command: ./pipex infile "grep a1" "wc -l" outfile
int	main(int argc, char **argv)
{
	int		fd;
	pid_t	pid;
	char	*args[4];
	int		d_pipe[2];

	if (argc != 5)
		return (1);
	build_args(args, argv[2]);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		exit(EXIT_FAILURE);
	if (pipe(d_pipe) == -1)
		exit(EXIT_FAILURE);
	close(fd);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		return (input_child_process(args, d_pipe));
	return (input_parent_process(pid, args, argv, d_pipe));
}
