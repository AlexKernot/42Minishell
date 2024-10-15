/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_env_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:01:58 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:24:47 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment_variables.h"

#include <stddef.h>

#include "libft.h"

t_env_vars	*make_env_var(t_env_vars *current, char *name, char *value)
{
	t_env_vars	*new;

	new = ft_calloc(1, sizeof(*current));
	if (new == NULL)
		return (new);
	if (current == NULL)
		*get_env_vars() = new;
	else
		current->next = new;
	new->name = name;
	new->val = value;
	new->next = NULL;
	return (new);
}

/**
 * @author Prachi Chawda
 * Creates a new environment variable set to null and pushes it onto
 * the front of the environment variable list.
*/
t_env_vars	*env_push_front(void)
{
	t_env_vars	*head;
	t_env_vars	*retval;

	head = *get_env_vars();
	retval = ft_calloc(1, sizeof(*retval));
	retval->next = head;
	*get_env_vars() = retval;
	return (retval);
}
