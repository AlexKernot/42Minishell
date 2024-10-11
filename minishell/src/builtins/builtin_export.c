/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:21:45 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 20:53:48 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "libft.h"
#include "builtin.h"
#include "expand.h"
#include "environment_variables.h"

/**
 * @author Prachi Chawda
 * STATIC:
 * loops over all environment variables and prints out name=value pairs,
 * or just name if value doesn't exist.
*/
static void	print_envs(void)
{
	const t_env_vars *env = *(get_env_vars());

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
static void	insert(t_env_vars *found, char *name, char *value)
{
	if (found->name == NULL)
		found->name = ft_strdup(name);
	if (found->val != NULL)
		free(found->val);
	found->val = ft_strdup(value);
	if (ft_strlen(found->val) == 0)
	{
		free(found->val);
		found->val = NULL;
	}
	return ;
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

static void	add_env_var(char *var)
{
	t_env_vars	*existing_env_var;
	char		*name;
	int			equal_sign_index;

	equal_sign_index = find_equal_sign(var);
	name = ft_strndup(var, equal_sign_index);
	existing_env_var = find_env_var(name);
	if (existing_env_var == NULL)
		existing_env_var = env_push_front();
	insert(existing_env_var, name, &var[equal_sign_index + 1]);
	free(name);
}

int	validate_env_var_name(char *name)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(name))
	{
		if (name[i] == '=' && (i == 0 || i == ft_strlen(name) - 1))
			return (1);
		if (name[i] == '=')
			return (0);
		if (i == 0 && ft_isalpha(name[i]) == 0)
			return (1);
		if (ft_isalnum(name[i]) == 0)
			return (1);
		++i;
	}
	return (0);
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
*/
int	builtin_export(int ac, char *av[])
{
	int	i;

	if (ac == 1)
		print_envs();
	i = 1;
	while (i < ac)
	{
		if (validate_env_var_name(av[i]) == 1)
		{
			write(2, "minishell: export: not a valid identifier\n",
				42);
			return (1);
		}
		add_env_var(av[i]);
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
