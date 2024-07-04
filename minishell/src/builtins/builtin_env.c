/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:53:24 by pchawda           #+#    #+#             */
/*   Updated: 2024/01/28 18:45:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "builtin.h"
#include "minishell.h"
#include "environment_variables.h"

/**
 * @author Prachi Chawda
 * @brief a simple recreation of the bash builtin 'env'. It takes no arguments
 * or options and prints out the list of environment variables, in the format
 * "name=value\n."
 * @param ac Should be 1. If this parameter does not equal 1 this function will
 * fail.
 * @param av This parameter is accepted for consistency between builtins 
 * however is not used in this function.
 * @return 0 on success or 1 on failure.
*/
int	builtin_env(int ac, char *av[])
{
	char	**env;
	int		i;

	if (ac != 1)
	{
		write(2, "Error: Env does not take an arguments\n", 39);
		return (1);
	}
	(void)av;
	env = compress_env_vars();
	if (*env == NULL)
	{
		write(2, "Error: Environment variables not available\n", 43);
		clear_array(env);
		return (1);
	}
	i = 0;
	while (i < env_len())
	{
		write(2, env[i], strlen(env[i]));
		write(2, "\n", 1);
		i++;
	}
	clear_array(env);
	return (0);
}
