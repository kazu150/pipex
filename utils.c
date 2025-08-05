/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:12:11 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/05 19:30:10 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	build_args(char *args[1000], char *command)
{
	char	**strs;
	int		i;

	i = 0;
	strs = ft_split(command, ' ');
	while (strs[i])
	{
		args[i] = strs[i];
		i++;
	}
	if (i >= 1000)
		exit(1);
	args[i] = NULL;
}

int	output_child_process(char **args, char *output_filename, int pipe_in)
{
	int		fd;
	char	*cmd;

	cmd = ft_strjoin("/bin/", args[0]);
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		error_exit(DUP2);
	close(pipe_in);
	fd = open(output_filename, O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		error_exit(output_filename);
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_exit(DUP2);
	close(fd);
	if (execve(cmd, args, 0) == -1)
		error_exit(EXECVE);
	return (0);
}

char	*build_command_path(char *cmd)
{
	char		*command_path;
	int			i;
	const char	*paths[6];

	paths[0] = "/usr/local/sbin/";
	paths[1] = "/usr/local/bin/";
	paths[2] = "/usr/sbin/";
	paths[3] = "/usr/bin/";
	paths[4] = "/sbin/";
	paths[5] = "/bin/";
	i = 0;
	while (i < 6)
	{
		command_path = ft_strjoin(paths[i], cmd);
		if (access(command_path, X_OK))
			break ;
		i++;
	}
	if (i == 6)
		error_exit(ACCESS);
	return (command_path);
}

int	input_child_process(char **args, char *input_file, int d_pipe[2])
{
	int		fd;
	char	*cmd;

	cmd = build_command_path(args[0]);
	close(d_pipe[0]);
	fd = open(input_file, O_RDONLY);
	if (fd == -1)
		error_exit(input_file);
	if (dup2(fd, STDIN_FILENO) == -1)
		error_exit(DUP2);
	close(fd);
	if (dup2(d_pipe[1], STDOUT_FILENO) == -1)
		error_exit(DUP2);
	close(d_pipe[1]);
	if (execve("/bin/ls", args, 0) == -1)
		error_exit(EXECVE);
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
		error_exit(FORK);
	if (o_pid == 0)
		return (output_child_process(args, output_filename, d_pipe[0]));
	else
	{
		if (waitpid(pid, &status, 0) < 0 || waitpid(o_pid, &o_status, 0) < 0)
			error_exit(WAITPID);
		if (WIFEXITED(status) && WIFEXITED(o_status))
		{
			close(d_pipe[0]);
			return (WEXITSTATUS(o_status));
		}
	}
	return (0);
}
