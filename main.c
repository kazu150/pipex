/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/26 15:17:07 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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

int	exec_output_child_process(char **args, char *output_command,
		char *output_filename)
{
	int	fd;
	int	tmp_fd;

	args[2] = output_command;
	tmp_fd = open("tmp", O_RDONLY);
	if (tmp_fd == -1)
		return (1);
	dup2(tmp_fd, STDIN_FILENO);
	if (tmp_fd == -1)
		return (1);
	fd = open(output_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (1);
	fd = dup2(fd, STDOUT_FILENO);
	if (fd == -1)
		return (1);
	if (execve("/bin/sh", args, 0) == -1)
		return (1);
	return (0);
}

int	exec_output_parent_process(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		unlink("./tmp");
	return (0);
}

int	exec_input_child_process(char **args)
{
	int	fd;

	fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (1);
	fd = dup2(fd, STDOUT_FILENO);
	if (fd == -1)
		return (1);
	if (execve("/bin/sh", args, 0) == -1)
		return (1);
	return (0);
}

int	exec_input_parent_process(pid_t pid, char **args, char *output_command,
		char *output_filename)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
	{
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
			return (exec_output_child_process(args, output_command,
					output_filename));
		else
			return (exec_output_parent_process(pid));
	}
	return (0);
}

//  command: ./a.out file1 "grep a1" "wc -l" file2
int	main(int argc, char **argv)
{
	int		fd;
	pid_t	pid;
	char	*args[4];

	if (argc != 5)
		return (1);
	build_args(args, argv[2]);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (1);
	fd = dup2(fd, STDIN_FILENO);
	if (fd == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		return (exec_input_child_process(args));
	else
		return (exec_input_parent_process(pid, args, argv[3], argv[4]));
}
