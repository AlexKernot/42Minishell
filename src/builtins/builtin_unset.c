/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:23:01 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:45:56 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		delete_env_var(env, args[i]);
		++i;
	}
	return (0);
}
