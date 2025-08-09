/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:12:11 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/09 15:18:42 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	error_exit(char *error_target)
{
	perror(error_target);
	exit(EXIT_FAILURE);
}

const char	**get_default_paths(void)
{
	static const char	*paths[7];

	paths[0] = "/usr/local/sbin/";
	paths[1] = "/usr/local/bin/";
	paths[2] = "/usr/sbin/";
	paths[3] = "/usr/bin/";
	paths[4] = "/sbin/";
	paths[5] = "/bin/";
	paths[6] = NULL;
	return (paths);
}

void	handle_command_path_error(char **args, int has_permission_error)
{
	char	*msg;

	if (has_permission_error)
	{
		msg = ft_strjoin(args[0], ": Permission denied\n");
		if (!msg)
			error_exit(MALLOC);
		ft_printf(msg);
		free(msg);
		free_split(args);
		exit(126);
	}
	else
	{
		msg = ft_strjoin(args[0], ": command not found\n");
		if (!msg)
			error_exit(MALLOC);
		ft_printf(msg);
		free(msg);
		free_split(args);
		exit(127);
	}
}

char	*build_command_path(char **args)
{
	char		*command_path;
	int			i;
	const char	**paths;
	int			has_permission_error;

	has_permission_error = 0;
	paths = get_default_paths();
	i = 0;
	while (i < 6)
	{
		command_path = ft_strjoin(paths[i], args[0]);
		if (!command_path)
			error_exit(MALLOC);
		if (access(command_path, X_OK) == 0)
			break ;
		if (errno == EACCES)
			has_permission_error = 1;
		free(command_path);
		i++;
	}
	if (i == 6)
		handle_command_path_error(args, has_permission_error);
	return (command_path);
}
