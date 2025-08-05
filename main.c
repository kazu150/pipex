/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/08/05 19:30:04 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(char *error_target)
{
	perror(error_target);
	exit(EXIT_FAILURE);
}

//  command: ./pipex infile "grep a1" "wc -l" outfile
int	main(int argc, char **argv)
{
	pid_t	pid;
	char	*args[1000];
	int		d_pipe[2];

	if (argc != 5)
		return (1);
	if (pipe(d_pipe) == -1)
		error_exit(PIPE);
	pid = fork();
	if (pid == -1)
		error_exit(FORK);
	if (pid == 0)
	{
		build_args(args, argv[2]);
		return (input_child_process(args, argv[1], d_pipe));
	}
	else
	{
		build_args(args, argv[3]);
		return (input_parent_process(pid, args, argv[4], d_pipe));
	}
}
