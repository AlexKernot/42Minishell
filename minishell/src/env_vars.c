/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 19:23:55 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:03:19 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "environment_variables.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @author Prachi Chawda
 * @brief uses a singleton design pattern to create a process-global variable
 * for use with storing environment variables in a modifiable way
 * @return the process-global environment variable list
*/
t_env_vars	**get_env_vars(void)
{
	static t_env_vars	*vars = NULL;

	return (&vars);
}

static void	add_pwd(void)
{
	t_env_vars	*current;
	t_env_vars	*pwd;
	char		cwd[128];

	current = *get_env_vars();
	while (current != NULL && current->next != NULL)
		current = current->next;
	pwd = find_env_var("PWD");
	getcwd(cwd, sizeof(cwd));
	if (pwd == NULL)
		make_env_var(current, ft_strdup("PWD"), ft_strdup(cwd));
	else
	{
		free(pwd->val);
		pwd->val = ft_strdup(cwd);
	}
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
	while (i < ft_arrlen((char **)envp))
	{
		size = find_equal_sign(envp[i]);
		if (strncmp(envp[i], "OLDPWD", 6) == 0)
		{
			current = make_env_var(current, ft_strdup("OLDPWD"), NULL);
			++i;
			continue ;
		}
		current = make_env_var(current, ft_substr(envp[i], 0, size),
				ft_substr(envp[i], size + 1,
					ft_strlen(envp[i]) - size));
		++i;
	}
	add_pwd();
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
		if (envp->val == NULL)
		{
			array[i] = ft_strdup(envp->name);
			++i;
			envp = envp->next;
			continue ;
		}
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
	(*get_env_vars()) = NULL;
}
