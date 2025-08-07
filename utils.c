/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:12:11 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/07 16:55:35 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

char	*build_command_path(char *cmd)
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
		command_path = ft_strjoin(paths[i], cmd);
		if (access(command_path, X_OK) == 0)
			break ;
		if (errno == EACCES)
			has_permission_error = 1;
		free(command_path);
		i++;
	}
	if (i == 6 && has_permission_error)
		(ft_printf(ft_strjoin(cmd, ": Permission denied\n")), exit(126));
	if (i == 6 && !has_permission_error)
		(ft_printf(ft_strjoin(cmd, ": command not found\n")), exit(127));
	return (command_path);
}

int	output_child_process(char **args, char *output_filename, int pipe_in)
{
	int		fd;
	char	*cmd;

	cmd = build_command_path(args[0]);
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(pipe_in);
	fd = open(output_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		(free(cmd), free_split(args), error_exit(output_filename));
	if (dup2(fd, STDOUT_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(fd);
	if (execve(cmd, args, 0) == -1)
		(free(cmd), free_split(args), error_exit(EXECVE));
	free_split(args);
	return (0);
}

int	input_child_process(char **args, char *input_file, int d_pipe[2])
{
	int		fd;
	char	*cmd;

	cmd = build_command_path(args[0]);
	close(d_pipe[0]);
	fd = open(input_file, O_RDONLY);
	if (fd == -1)
		(free(cmd), free_split(args), error_exit(input_file));
	if (dup2(fd, STDIN_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(fd);
	if (dup2(d_pipe[1], STDOUT_FILENO) == -1)
		(free(cmd), free_split(args), error_exit(DUP2));
	close(d_pipe[1]);
	if (execve(cmd, args, 0) == -1)
		(free(cmd), free_split(args), error_exit(EXECVE));
	else
		free_split(args);
	return (0);
}

int	input_parent_process(pid_t pid, char **args, char *output_filename,
		int d_pipe[2])
{
	int		status;
	int		o_status;
	pid_t	o_pid;

	close(d_pipe[1]);
	o_pid = fork();
	if (o_pid < 0)
		(free_split(args), error_exit(FORK));
	if (o_pid == 0)
		return (output_child_process(args, output_filename, d_pipe[0]));
	else
	{
		free_split(args);
		if (waitpid(pid, &status, 0) < 0 || waitpid(o_pid, &o_status, 0) < 0)
			(free_split(args), error_exit(WAITPID));
		if (WIFEXITED(status) && WIFEXITED(o_status))
		{
			close(d_pipe[0]);
			return (WEXITSTATUS(o_status));
		}
	}
	return (0);
}
