/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/08/05 15:01:01 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//  command: ./pipex infile "grep a1" "wc -l" outfile
int	main(int argc, char **argv)
{
	pid_t	pid;
	char	*args[4];
	int		d_pipe[2];

	if (argc != 5)
		return (1);
	build_args(args, argv[2]);
	if (pipe(d_pipe) == -1)
		error_exit(PIPE);
	pid = fork();
	if (pid == -1)
		error_exit(FORK);
	if (pid == 0)
		return (input_child_process(args, argv, d_pipe));
	else
		return (input_parent_process(pid, args, argv, d_pipe));
}
