/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:21:45 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:45:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "libft.h"
#include "builtin.h"
#include "environment_variables.h"

/**
 * @author Prachi Chawda
 * STATIC:
 * loops over all environment variables and prints out name=value pairs,
 * or just name if value doesn't exist.
*/
static void	print_envs(t_env_vars *env)
{
	while (env != NULL)
	{
		if (env->name == NULL)
		{
			env = env->next;
			continue ;
		}
		else if (env->val == NULL)
			printf("declare -x %s\n", env->name);
		else
			printf("declare -x %s=\"%s\"\n", env->name, env->val);
		env = env->next;
	}
}

/**
 * @author Prachi Chawda
 * STATIC:
 * if an equal sign exists, seperate the name and value from toInsert and
 * insert them into their fields respectively. Otherwise set name and set
 * value to NULL.
*/
static void	insert(t_env_vars *found, char *name, char *toInsert)
{
	int	equal_index;
	int	stringlen;
	int	size_of_value;

	equal_index = find_equal_sign(toInsert);
	stringlen = (int)ft_strlen(toInsert);
	size_of_value = stringlen - equal_index - 1;
	if (found->name == NULL)
		found->name = ft_strdup(name);
	if (found->val != NULL)
		free(found->val);
	if (equal_index != -1)
	{
		found->val = ft_substr(toInsert, equal_index + 1, size_of_value);
		if (ft_strlen(found->val) == 0)
		{
			free(found->val);
			found->val = NULL;
		}
		return ;
	}
	found->val = NULL;
}

/**
 * @author Prachi Chawda
 * STATIC:
 * returns a newly allocated string just containing the name of an environment.
 * It removes everything after an equal sign.
*/
static char	*seperate_name(char *av)
{
	int	equal_index;

	equal_index = find_equal_sign(av);
	if (equal_index == -1)
		return (ft_strdup(av));
	return (ft_substr(av, 0, equal_index));
}

/**
 * @author Prachi Chawda
 * STATIC:
 * Creates a new environment variable set to null and pushes it onto
 * the front of the environment variable list.
*/
static t_env_vars	*env_push_front(void)
{
	t_env_vars	*head;
	t_env_vars	*retval;

	head = *get_env_vars();
	retval = ft_calloc(1, sizeof(*retval));
	retval->next = head;
	*get_env_vars() = retval;
	return (retval);
}

/**
 * @author Prachi Chawda
 * @brief a recreation of the bash builtin 'export'. If no arguments are passed
 * into this command, it will print out the list of environment variables in the
 * form 'export -d NAME="VALUE"'. Otherwise, it will search the list for each
 * environment variable passed into av. If it exists, then overwrite the variable
 * with a new VALUE. If no VALUE is specified, the value will be set to NULL. If
 * the environment variable doesn't already exist, it will create it and append
 * it onto the front of the list.
 * @param ac the number of arguments passed to this function in av, including
 * the command name itself. This argument should always be greater than 0.
 * @param av an array of strings containing the arguments being passed to this
 * command. It should start with the command name in index 0, and the array
 * should be terminated by a NULL value.
 * @return 0 is the command succeeded, or 1 or failure.
 * @remark If I had 1 line spare, I could reverse the if clause to make it much
 * cleaner...
*/
int	builtin_export(int ac, char *av[])
{
	t_env_vars	*env;
	t_env_vars	*found;
	char		*name;
	int			i;

	env = *get_env_vars();
	if (ac == 1)
		print_envs(env);
	i = 1;
	while (i < ac)
	{
		if (av[i][0] != '=')
		{
			name = seperate_name(av[i]);
			found = find_env_var(name);
			if (found == NULL)
				found = env_push_front();
			insert(found, name, av[i]);
			free(name);
		}
		else
			write(2, "Can not set env name to '='.\n", 30);
		++i;
	}
	return (0);
}
/*
int main(int ac, char *av[], char *const envp[])
{
	init_env_vars(envp);
	builtin_export(ac, av);
	if (ac > 1)
	{
		for (int i = 1; i < ac; ++i) {
			char	*name = seperate_name(av[i]);
			printf("%s = %s\n", name, ft_getenv(name));
			free(name);
		}
	}
	clear_env_vars();
	return (0);
}
*/
