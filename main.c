/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/23 18:21:26 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char	*build_command(char *command, char *file)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	while (command[i])
		i++;
	while (file[j])
		j++;
	result = malloc(sizeof(int) * (i + j + 2));
	i = 0;
	while (command[i])
	{
		result[i] = command[i];
		i++;
	}
	result[i++] = ' ';
	j = 0;
	while (file[j])
	{
		result[i] = file[j];
		i++;
		j++;
	}
	return (result);
}

//  ./a.out file1 "grep a1" |  wc -w > file2 ここまではできる
int	main(int argc, char **argv)
{
	int		fd;
	char	*args[4];
	pid_t	pid1;
	pid_t	pid2;
	int		pfd[2];
	int		ret_val;
	int		saved_stdout;
	int		status;
	int		tmp_fd;

	ret_val = pipe(pfd);
	args[0] = "sh";
	args[1] = "-c";
	args[2] = argv[2];
	args[3] = NULL;
	saved_stdout = dup(STDOUT_FILENO);
	if (argc < 3)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (!fd)
		return (1);
	fd = dup2(fd, STDIN_FILENO);
	if (fd == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	printf("1: pid is %d\n", pid1);
	if (pid1 == 0)
	{
		fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
		execve("/bin/sh", args, 0);
	}
	else
	{
		waitpid(pid1, &status, 0);
		if (WIFEXITED(status))
		{
			pid2 = fork();
			if (pid2 < 0)
				return (1);
			if (pid2 == 0)
			{
				args[2] = argv[3];
				tmp_fd = open("tmp", O_RDONLY);
				fd = open(argv[4], O_WRONLY | O_CREAT, 0644);
				printf("[%s]", args[2]);
				printf("2: pid is %d\n", pid2);
				dup2(STDOUT_FILENO, fd);
				dup2(STDIN_FILENO, tmp_fd);
				execve("/bin/sh", args, 0);
			}
		}
	}
	// else
	// {
	// 	printf("args[2] is ?");
	// 	args[2] = build_command(argv[3], argv[4]);
	// 	dup2(pfd[1], 1);
	// 	execve("/bin/sh", args, 0);
	// }
	return (0);
}
