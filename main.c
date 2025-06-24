/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/24 14:07:11 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//  command: ./a.out file1 "grep a1" |  "wc -l" file2
int	main(int argc, char **argv)
{
	int		fd;
	pid_t	pid;
	char	*args[4];
	int		status;
	int		tmp_fd;

		args[0] = "sh";
		args[1] = "-c";
		args[2] = argv[2];
		args[3] = NULL;
	if (argc < 5)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (!fd)
		return (1);
	fd = dup2(fd, STDIN_FILENO);
	if (fd == -1)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
		execve("/bin/sh", args, 0);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			pid = fork();
			if (pid < 0)
				return (1);
			if (pid == 0)
			{
				args[2] = argv[3];
				tmp_fd = open("tmp", O_RDONLY);
				fd = open(argv[4], O_WRONLY | O_CREAT, 0644);
				dup2(fd, STDOUT_FILENO);
				dup2(tmp_fd, STDIN_FILENO);
				execve("/bin/sh", args, 0);
			}
			else
			{
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					unlink("./tmp");
			}
		}
	}
	return (0);
}
