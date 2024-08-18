/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 14:29:02 by akernot           #+#    #+#             */
/*   Updated: 2024/08/18 17:39:22 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "libft.h"
#include "environment_variables.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @author Prachi Chawda
 * STATIC:
 * Checks if a '~' should be expanded into /home/name by checking if a directory
 * or file exists in the current working directory with the name '~'. If not, it
 * then checks if the path is '~' by itself, replacing it with $HOME. It finally
 * checks if the path starts with "~/" and if it does, it replaces the '~' with
 * the $HOME environment variable. If none if that is true, it just returns a
 * newly allocated string containing path.
*/
static char	*expand_directory(char *path)
{
	if (path[0] == '~' && access("~", F_OK) == -1)
	{
		if (path[1] == '\0')
			return (ft_strdup(ft_getenv("HOME")));
		if (path[1] == '/')
			return (ft_strjoin(ft_getenv("HOME"), &path[1]));
	}
	return (ft_strdup(path));
}

/**
 * @author Prachi Chawda
 * @brief A recreation of the bash CD builtin. It changes the working directory
 * to that specified in the first argument passed. If no arguments are passed to
 * it, this function will set the working directory to the directory set in the
 * $HOME environment variable.
 * @param ac the number of arguments passed in av, including the function name
 * itself.
 * @param av an array of char pointers containing the arguments for the
 * function. The first argument in always the command ('cd') and the following
 * string should be the path to set the working directory to. If more than 2
 * arguments are passed to av, the function will fail.
 * @warning This function changes the working directory for the current running
 * process. This may have unintended side effects and also wont change the
 * directory for parent or child processes.
 * @return 0 on success, and -1 on failure.
*/
int	builtin_cd(int ac, char *av[])
{
	char	*expanded_dir;
	char	cwd[255];

	if (ac == 1)
	{
		chdir(getenv("HOME"));
		return (0);
	}
	if (ac > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return (1);
	}
	expanded_dir = expand_directory(av[1]);
	if (chdir(expanded_dir) == -1)
	{
		perror("minishell: cd");
		free(expanded_dir);
		return (1);
	}
	free(find_env_var("PWD")->val);
	getcwd(cwd, 255);
	find_env_var("PWD")->val = ft_strdup(cwd);
	free(expanded_dir);
	return (0);
}
