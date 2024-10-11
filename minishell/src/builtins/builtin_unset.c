/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:23:01 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 18:24:38 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libft.h"
#include "builtin.h"
#include "environment_variables.h"

/**
 * @author Prachi Chawda
*/
static void	delete_env_var(t_env_vars *env, char *name)
{
	t_env_vars	*current;
	t_env_vars	*last;
	size_t		name_len;

	name_len = ft_strlen(name);
	current = env;
	last = current;
	while (current != NULL)
	{
		if (current->name != NULL
			&& strncmp(current->name, name, name_len) == 0
			&& name_len == ft_strlen(current->name))
		{
			if (current == *get_env_vars())
				*get_env_vars() = current->next;
			else
				last->next = current->next;
			free(current->name);
			free(current->val);
			free(current);
			return ;
		}
		last = current;
		current = current->next;
	}
}

/**
 * @author Prachi Chawda
*/
int	builtin_unset(int ac, char *args[])
{
	t_env_vars	*env;
	int			i;

	env = *get_env_vars();
	if (ac == 1)
		return (0);
	i = 1;
	while (i < ac)
	{
		if (validate_env_var_name(args[i]) == 1)
		{
			write(STDERR_FILENO, "minishell: unset: `", 19);
			write(STDERR_FILENO, args[i], ft_strlen(args[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			return (1);
		}
		delete_env_var(env, args[i]);
		++i;
	}
	return (0);
}
