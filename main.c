/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 23:16:06 by vscode            #+#    #+#             */
/*   Updated: 2025/08/12 17:43:10 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//  command: ./pipex infile "grep a1" "wc -l" outfile
int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	char	**args;
	int		d_pipe[2];

	args = NULL;
	if (argc != 5)
		exit(1);
	if (pipe(d_pipe) == -1)
		error_exit(PIPE);
	pid = fork();
	if (pid == -1)
		error_exit(FORK);
	if (pid == 0)
	{
		args = ft_split(argv[2], ' ');
		if (!args)
			error_exit(MALLOC);
		return (input_child_process(args, argv[1], d_pipe, envp));
	}
	args = ft_split(argv[3], ' ');
	if (!args)
		error_exit(MALLOC);
	return (output(pid, args, argv[4], d_pipe, envp));
}
