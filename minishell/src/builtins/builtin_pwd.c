/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:55:29 by pchawda           #+#    #+#             */
/*   Updated: 2024/01/28 18:45:43 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

#include "builtin.h"

/**
 * @author Prachi Chawda
*/
int	builtin_pwd(int ac, char *av[])
{
	char	cwd[4096];

	(void)ac;
	(void)av;
	if (!getcwd(cwd, 4096))
	{
		return (1);
	}
	if (write(1, cwd, strlen(cwd)) >= 0 && write(1, "\n", 1) >= 0)
	{
		return (0);
	}
	return (1);
}
