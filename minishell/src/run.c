/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:18:59 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 16:33:26 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "run.h"
#include "redirect.h"
#include "syntax_tree.h"
#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

/** 
 * @author Alex Kernot
 * STATIC:
 * checks if a command is a builtin by hashing the string then comparing it to
 * the predefined hash values for each builtin. It also checks the length of the
 * command just in case of hash collisions.
*/
t_bool	is_builtin(const char *str)
{
	if (ft_strncmp("echo", str, 4) == 0)
		return (true);
	if (ft_strncmp("pwd", str, 3) == 0)
		return (true);
	if (ft_strncmp("export", str, 6) == 0)
		return (true);
	if (ft_strncmp("cd", str, 2) == 0)
		return (true);
	if (ft_strncmp("unset", str, 5) == 0)
		return (true);
	if (ft_strncmp("env", str, 3) == 0)
		return (true);
	if (ft_strncmp("exit", str, 4) == 0)
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
int	run_path(char **substr)
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
	write(2, "Minishell: ", 11);
	write(2, *substr, ft_strlen(*substr));
	write(2, ": Command not found\n", 20);
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
int	run_without_subshell(t_command *command)
{
	int		pid;
	int		retval;
	const int	old_stdin = dup(STDIN_FILENO);
	const int	old_stdout = dup(STDOUT_FILENO);
	

	if (redirect(command) == 1)
		return (1);
	if (is_builtin(command->command))
	{
		retval = run_builtin(command->args->array);
		dup2(old_stdin, STDIN_FILENO);
		dup2(old_stdout, STDOUT_FILENO);
		return (retval);
	}
	pid = fork();
	if (pid != 0)
	{
		waitpid(pid, &retval, 0);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		dup2(old_stdin, STDIN_FILENO);
		dup2(old_stdout, STDOUT_FILENO);
		return (process_exit_status(retval));
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	run_path(command->args->array);
	exit(0);
}

/**
 * @author Alex Kernot
 * @brief runs command in a subshell. It checks if the command is a builtin or
 * exists in the current directory before searching path
 * @param segment the entire command to execute including expanded environment
 * variables and information about each redirect required.
*/
void	run_command(t_command *command)
{
	const char	*cmd = command->command;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (redirect(command) == 1)
		exit(1);
	if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
		execute(cmd, command->args->array);
	if (is_builtin(command->command))
		exit(run_builtin(command->args->array));
	run_path(command->args->array);
	exit(0);
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
