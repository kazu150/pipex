/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/06/28 19:22:34 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		error_exit(argv[1]);
	if (dup2(fd, STDIN_FILENO) == -1)
		error_exit(DUP2);
	if (pipe(d_pipe) == -1)
		error_exit(PIPE);
	close(fd);
	pid = fork();
	if (pid == -1)
		error_exit(FORK);
	if (pid == 0)
		return (input_child_process(args, d_pipe));
	return (input_parent_process(pid, args, argv, d_pipe));
}
