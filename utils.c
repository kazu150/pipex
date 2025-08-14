/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:12:11 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/14 12:19:53 by vscode           ###   ########.fr       */
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

char	*pathjoin(const char *path1, const char *path2)
{
	char	*with_slash;
	char	*full_path;

	with_slash = ft_strjoin(path1, "/");
	if (!with_slash)
		error_exit(MALLOC);
	full_path = ft_strjoin(with_slash, path2);
	if (!full_path)
		error_exit(MALLOC);
	free(with_slash);
	return (full_path);
}

char	*build_command_path(char **args, char **envp)
{
	char	*command_path;
	int		i;
	char	**paths;
	int		has_permission_error;

	if (args[0][0] == '/')
		return (args[0]);
	has_permission_error = 0;
	paths = get_default_paths(envp);
	i = 0;
	while (paths && paths[i])
	{
		command_path = pathjoin(paths[i], args[0]);
		if (!command_path)
			error_exit(MALLOC);
		if (access(command_path, X_OK) == 0)
			break ;
		if (errno == EACCES)
			has_permission_error = 1;
		free(command_path);
		i++;
	}
	if (!paths || !paths[i])
		handle_command_path_error(args, has_permission_error);
	return (command_path);
}
