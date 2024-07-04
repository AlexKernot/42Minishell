/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:46:09 by pchawda           #+#    #+#             */
/*   Updated: 2024/01/28 18:45:07 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"

/**
 * @author Prachi Chawda
 * 
*/
int	builtin_echo(int ac, char *av[])
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (i < ac && strcmp(av[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (i < ac)
	{
		write(1, av[i], strlen(av[i]));
		if (i + 1 < ac && av[i][0] != '\0')
		{
			write(1, " ", 1);
		}
		i++;
	}
	if (n_flag == 0)
	{
		write(1, "\n", 1);
	}
	return (0);
}
