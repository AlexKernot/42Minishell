/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 19:23:55 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:44:25 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "environment_variables.h"

#include <stdlib.h>

/**
 * @author Prachi Chawda
 * @details I understand this isn't a good way of handling environment variables
 * however, the 42 style guide (Norminette) is too restrictive, especially with
 * a parameter limit of 4, and function line limit of 25. To elegantly make 
 * another solution, I would need to do the equivalent of dependancy injection
 * which would use one of my 4 arguments in just about every function.
 * @brief uses a singleton design pattern to create a process-global variable
 * for use with storing environment variables in a modifiable way
 * @return the process-global environment variable list
*/
t_env_vars	**get_env_vars(void)
{
	static t_env_vars	*vars;

	if (vars == NULL)
		vars = ft_calloc(1, sizeof(*vars));
	return (&vars);
}

/**
 * @author Prachi Chawda
 * @brief creates an environment variable linked list from envp that can be
 * freely modified and compressed back into an array of strings
 * @param envp the array of program environment variables in the form
 * "name:value1;...;valueN;"
*/
void	init_env_vars(char *const envp[])
{
	t_env_vars	*current;
	int			i;
	int			size;

	current = *get_env_vars();
	i = 0;
	while (1)
	{
		size = find_equal_sign(envp[i]);
		current->name = ft_substr(envp[i], 0, size);
		current->val = ft_substr(envp[i], size + 1, ft_strlen(envp[i]) - size);
		++i;
		if (i >= ft_arrlen((char **)envp))
			break ;
		current->next = ft_calloc(1, sizeof(*current));
		current = current->next;
	}
	current->next = NULL;
}

/**
 * @author Prachi Chawda
 * @brief compresses the linked list into a newly allocated array of strings.
 * @return this function returns an array of strings containing all the
 * environent variables supplied
*/
char	**compress_env_vars(void)
{
	t_env_vars	*envp;
	char		**array;
	char		*tmp;
	int			i;

	envp = *get_env_vars();
	array = ft_calloc(env_len() + 1, sizeof(*envp));
	i = 0;
	while (envp != NULL)
	{
		tmp = ft_strjoin(envp->name, "=");
		array[i] = ft_strjoin(tmp, envp->val);
		free(tmp);
		++i;
		envp = envp->next;
	}
	array[i] = NULL;
	return (array);
}

/**
 * @author Prachi Chawda
 * @brief frees all the data within this process' environment variable structure
*/
void	clear_env_vars(void)
{
	t_env_vars	*current;
	t_env_vars	*next;

	current = *get_env_vars();
	while (current != NULL)
	{
		if (current->name != NULL)
			free(current->name);
		if (current->val != NULL)
			free(current->val);
		next = current->next;
		free(current);
		current = next;
	}
}
