/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:12:11 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/12 19:18:08 by vscode           ###   ########.fr       */
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

char	**get_default_paths(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			envp[i] = envp[i] + 5;
			paths = ft_split(envp[i], ':');
			if (!paths)
				error_exit(MALLOC);
			return (paths);
		}
		i++;
	}
	return (paths);
}

void	handle_command_path_error(char **args, int has_permission_error)
{
	char	*str;
	int		len;

	if (has_permission_error)
	{
		str = ft_strjoin(args[0], ": Permission denied\n");
		if (!str)
			error_exit(MALLOC);
		len = ft_strlen(str);
		write(2, str, len);
		free_split(args);
		exit(126);
	}
	else
	{
		str = ft_strjoin(args[0], ": command not found\n");
		if (!str)
			error_exit(MALLOC);
		len = ft_strlen(str);
		write(2, str, len);
		free_split(args);
		exit(127);
	}
}

char	*build_command_path(char **args, char **envp)
{
	char	*slash;
	char	*command_path;
	int		i;
	char	**paths;
	int		has_permission_error;

	has_permission_error = 0;
	paths = get_default_paths(envp);
	i = 0;
	while (paths[i])
	{
		slash = ft_strjoin(paths[i], "/");
		if (!slash)
			error_exit(MALLOC);
		command_path = ft_strjoin(slash, args[0]);
		if (!command_path)
			error_exit(MALLOC);
		free(slash);
		if (access(command_path, X_OK) == 0)
			break ;
		if (errno == EACCES)
			has_permission_error = 1;
		free(command_path);
		i++;
	}
	if (!paths[i])
		handle_command_path_error(args, has_permission_error);
	return (command_path);
}
