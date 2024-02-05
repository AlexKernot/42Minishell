/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:18:59 by akernot           #+#    #+#             */
/*   Updated: 2024/02/02 00:29:55 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "segment.h"
#include "run.h"
#include "redirect.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdnoreturn.h>

/** 
 * @author Alex Kernot
 * STATIC:
 * checks if a command is a builtin by hashing the string then comparing it to
 * the predefined hash values for each builtin. It also checks the length of the
 * command just in case of hash collisions.
*/
static t_bool	is_builtin(const char *str)
{
	const int	hash = hash_func(str);
	const int	strlen = ft_strlen(str);

	if (hash == echo && strlen == 4)
		return (true);
	if (hash == pwd && strlen == 3)
		return (true);
	if (hash == export && strlen == 6)
		return (true);
	if (hash == cd && strlen == 2)
		return (true);
	if (hash == unset && strlen == 5)
		return (true);
	if (hash == env && strlen == 3)
		return (true);
	if (hash == eexit && strlen == 4)
		return (true);
	return (false);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * allocates memory for a new string containing 'path/file'
 * and returns the filled string to try execute the command.
*/
static char	*construct_path(char *path, const char *file)
{
	const int	strlen = ft_strlen(path);
	char		*tmp;
	char		*tmp2;

	if (path[strlen - 1] == '/')
		return (ft_strjoin(path, file));
	tmp = ft_strjoin(path, "/");
	tmp2 = ft_strjoin(tmp, file);
	free(tmp);
	return (tmp2);
}

/**
 * @author Alex Kernot
 * @brief searches path for the command. It searches each directory in path
 * and checks if a file with the name of the command exists. If it does, it
 * executes that program through execve. If no file exists with 
*/
static _Noreturn int	run_path(const char **substr)
{
	const char	*path_env = getenv("PATH");
	char		**paths;
	char		*command;
	int			perms;
	int			i;

	i = 0;
	if (substr[0][0] == '/' || substr[0][0] == '.' || substr[0][0] == '~')
		execute(substr[0], substr);
	paths = ft_split(path_env, ':');
	while (ft_strlen(*substr) != 0 && paths[i] != NULL)
	{
		command = construct_path(paths[i], *substr);
		perms = access(command, R_OK | X_OK);
		if (perms == 0)
		{
			execute(command, substr);
		}
		free(command);
		++i;
	}
	write(2, "Command '", 10);
	write(2, *substr, ft_strlen(*substr));
	write(2, "' not found\n", 13);
	exit(127);
}

/**
 * @author Alex Kernot
 * @brief if there is only 1 command total to be run, this command will be
 * called. This only creates a fork if there is going to be an execve call. This
 * allows builtins to freely modify the main shell's data, like environemnt
 * variables and the current working directory. This allows changes to persist
 * throughout the lifetime of the shell.
 * @param segment is the command to run. It contains the command, its arguments,
 * as well as any information about redirects.
 * @return the exit status of the command that was run.
*/
int	run_without_subshell(t_segment *segment)
{
	const char	*command = segment->command[0];
	int			pid;
	int			retval;

	dup2(STDIN_FILENO, 15);
	dup2(STDOUT_FILENO, 16);
	if (redirect(segment) == 1)
		return (1);
	if (is_builtin(command))
	{
		retval = run_builtin((const char **)segment->command);
		dup2(15, STDIN_FILENO);
		dup2(16, STDOUT_FILENO);
		return (retval);
	}
	pid = fork();
	if (pid != 0)
	{
		waitpid(pid, &retval, 0);
		return (process_exit_status(retval));
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	run_path((const char **)segment->command);
}

/**
 * @author Alex Kernot
 * @brief runs command in a subshell. It checks if the command is a builtin or
 * exists in the current directory before searching path
 * @param segment the entire command to execute including expanded environment
 * variables and information about each redirect required.
*/
_Noreturn void	run_command(t_segment *segment)
{
	const char	*command = segment->command[0];

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (redirect(segment) == 1)
		exit(1);
	if (command[0] == '/' || command[0] == '.' || command[0] == '~')
		execute(command, (const char **)segment->command);
	if (is_builtin(command))
		exit(run_builtin((const char **)segment->command));
	run_path((const char **)segment->command);
}

/*
int	main(void)
{
	printf("echo %i\n", hash("echo"));
	printf("pwd %i\n", hash("pwd"));
	printf("export %i\n", hash("export"));
	printf("cd %i\n", hash("cd"));
	printf("unset %i\n", hash("unset"));
	printf("env %i\n", hash("env"));
	printf("exit %i\n", hash("exit"));
}*/
