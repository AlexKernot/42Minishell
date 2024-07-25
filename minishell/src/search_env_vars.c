/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:44:07 by akernot           #+#    #+#             */
/*   Updated: 2024/07/21 16:57:55 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "environment_variables.h"

/**
 * @author Prachi Chawda
 * @brief finds the length of the environment variables list
 * @return returns the length of the environment variables list
*/
int	env_len(void)
{
	t_env_vars	*envp;
	int			i;

	i = 0;
	envp = *get_env_vars();
	while (envp != NULL)
	{
		envp = envp->next;
		++i;
	}
	return (i);
}

/**
 * @author Prachi Chawda
 * @brief searches the environment variable list for the specified environment
 * variable and returns it.
 * @param name the name of the environment variable to find
 * @return a pointer to the environment variable, or NULL if not found.
*/
t_env_vars	*find_env_var(char *name)
{
	t_env_vars	*current;
	int			name_strlen;
	int			current_strlen;

	if (name == NULL)
		return (NULL);
	current = *get_env_vars();
	name_strlen = ft_strlen(name);
	while (current != NULL)
	{
		current_strlen = ft_strlen(current->name);
		if (ft_strncmp(name, current->name, name_strlen) == 0
			&& current_strlen == name_strlen)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * @author Prachi Chawda
 * @brief meant to function identically to the C stdlibrary version of getenv
 * searches the list of environment variables for name and returns its value,
 * or NULL if not found or no value exists. Modifying the returned value will
 * modify the environment variable itself.
 * @param name the name of the environment variable to find.
 * @return a char pointer to the value of the environment variable, or NULL.
*/
char	*ft_getenv(char	*const name)
{
	t_env_vars	*found;

	if (name == NULL)
		return (NULL);
	found = find_env_var(name);
	if (found == NULL)
		return (NULL);
	return (found->val);
}

/**
 * @author Prachi Chawda
 * @brief searches string for next occurance of an equal sign and returns the
 * index it occurs.
 * @param string the string to search
 * @return the index of the next equal sign in string, or -1 if not found.
*/
int	find_equal_sign(char *const string)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(string))
	{
		if (string[i] == '=')
			return (i);
		++i;
	}
	return (-1);
}
