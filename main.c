/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/21 16:15:45 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//  ./a.out file1 "grep a1" |  wc -w > file2 ここまではできる
int	main(int argc, char **argv)
{
	int		fd;
	char	*args[4];
	pid_t	pid;

	args[0] = "sh";
	args[1] = "-c";
	args[2] = argv[2];
	args[3] = NULL;
	if (argc < 3)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (!fd)
		return (1);
	fd = dup2(fd, 0);
	if (fd == -1)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		execve("/bin/sh", args, 0);

		pid = fork();
		if (pid == 0)
		{
			
		}

		return (0);
	}
	return (0);
}
