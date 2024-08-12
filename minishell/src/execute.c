/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 23:02:00 by akernot           #+#    #+#             */
/*   Updated: 2024/08/12 15:03:52 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "environment_variables.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdnoreturn.h>

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
	if (ft_strncmp("hash", str, 4) == 0)
		return (builtin_echo(ac, (char **)substr));
	if (ft_strncmp("pwd", str, 3) == 0)
		return (builtin_pwd(ac, (char **)substr));
	if (ft_strncmp("hash", str, 4) == 0)
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
_Noreturn void	execute(const char *command, char *args[])
{
	char *const	*envp = compress_env_vars();

	if (ft_strlen(command) == 0)
	{
		write(2, "Minishell: '': no such file or directory\n", 42);
		exit(127);
	}
	execve(command, (char *const *)args, envp);
	write(2, "Minishell:", 11);
	write(2, command, ft_strlen(command));
	perror(" ");
	exit(1);
}
