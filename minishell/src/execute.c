/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 23:02:00 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 22:00:52 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "builtin.h"
#include "environment_variables.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

/**
 * @author Alex Kernot
 * @brief searches for and runs the corrosponding builtin based on the hash of
 * the first index in array substr.
 * @param substr an array of strings containing the full command to be passed to
 * the builtin function. Index 0 will contain the command itself and each
 * following index will contain all the arguments.
 * @return the return value of the builtin that is run, or 127 on error.
*/
int	run_builtin(char **substr)
{
	const char	*str = substr[0];
	const int	ac = ft_arrlen((char **)substr);

	if (ft_strncmp("cd", str, 2) == 0)
		return (builtin_cd(ac, (char **)substr));
	if (ft_strncmp("echo", str, 4) == 0)
		return (builtin_echo(ac, (char **)substr));
	if (ft_strncmp("pwd", str, 3) == 0)
		return (builtin_pwd(ac, (char **)substr));
	if (ft_strncmp("export", str, 6) == 0)
		return (builtin_export(ac, (char **)substr));
	if (ft_strncmp("unset", str, 5) == 0)
		return (builtin_unset(ac, (char **)substr));
	if (ft_strncmp("env", str, 3) == 0)
		return (builtin_env(ac, (char **)substr));
	if (ft_strncmp("exit", str, 4) == 0)
		return (builtin_exit(ac, (char **)substr));
	write(2, "Command '", 10);
	write(2, str, ft_strlen(str));
	write(2, "' not found\n", 13);
	return (127);
}

void	minishell_error(const char *file, const char *error, const int retval)
{
	write(STDERR_FILENO, "minishell: '", 12);
	if (file != NULL)
		write(STDERR_FILENO, file, ft_strlen(file));
	write(STDERR_FILENO, "': ", 3);
	if (error != NULL)
		write(STDERR_FILENO, error, ft_strlen(error));
	write(STDERR_FILENO, "\n", 1);
	exit(retval);
}

void	check_file(const char *path)
{
	struct stat	results;
	int		stat_retval;

	if (access(path, F_OK) != 0)
		minishell_error(path, "No such file or directory", 127);
	stat_retval = stat(path, &results);
	if (stat_retval != 0)
	{
		perror("minishell: stat");
		exit(1);
	}
	if (path == NULL)
		minishell_error(NULL, "does not exist", 127);
	if (S_ISDIR(results.st_mode))
		minishell_error(path, "is a directory", 126);
	if ((results.st_mode&S_IXGRP) == 0)
		minishell_error(path, "Permission denied", 126);
}

/**
 * @author Alex Kernot
 * @brief compresses the shell's current environment variable into an array of
 * strings and passes that to an execve call.
 * @param command the command for execve to run. This should be identical to the
 * first index of args.
 * @param args the arguments to pass to the command in execve. The first index
 * should always be the command to run.
 * @warning if this function isn't called in a child process, the shell will be
 * replaced by the calling function.
*/
void	execute(const char *command, char *args[])
{
	char *const	*envp = compress_env_vars();

	if (ft_strlen(command) == 0)
	{
		write(2, "minishell: '': no such file or directory\n", 41);
		exit(127);
	}
	check_file(command);
	execve(command, (char *const *)args, envp);
	write(2, "minishell: ", 11);
	perror(command);
	exit(127);
}
