/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/26 18:31:22 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	build_args(char *args[4], char *input_command)
{
	args[0] = "sh";
	args[1] = "-c";
	args[2] = input_command;
	args[3] = NULL;
}

int	exec_output_c_process(char **args, char *output_command,
		char *output_filename, int pipe_in)
{
	int	fd;

	args[2] = output_command;
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(pipe_in);
	fd = open(output_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		exit(EXIT_FAILURE);
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	if (execve("/bin/sh", args, 0) == -1)
		exit(EXIT_FAILURE);
	return (0);
}

int	exec_input_c_process(char **args, int d_pipe[2])
{
	close(d_pipe[0]);
	if (dup2(d_pipe[1], STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(d_pipe[1]);
	if (execve("/bin/sh", args, 0) == -1)
		exit(EXIT_FAILURE);
	return (0);
}

int	exec_input_p_process(pid_t pid, char **args, char **argv, int d_pipe[2])
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		exit(EXIT_FAILURE);
	if (WIFEXITED(status))
	{
		close(d_pipe[1]);
		pid = fork();
		if (pid < 0)
			exit(EXIT_FAILURE);
		if (pid == 0)
			return (exec_output_c_process(args, argv[3], argv[4], d_pipe[0]));
		else
		{
			close(d_pipe[0]);
			return (0);
		}
	}
	return (0);
}

//  command: ./a.out file1 "grep a1" "wc -l" file2
int	main(int argc, char **argv)
{
	int		fd;
	pid_t	pid;
	char	*args[4];
	int		d_pipe[2];

	pipe(d_pipe);
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
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		return (exec_input_c_process(args, d_pipe));
	else
		return (exec_input_p_process(pid, args, argv, d_pipe));
}
