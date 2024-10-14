/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:54:08 by pchawda           #+#    #+#             */
/*   Updated: 2024/10/14 17:41:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "libft.h"
#include "builtin.h"
#include "ft_atol.h"

t_bool is_number(const char *input)
{
	int		i;
	char	a;

	i = 0;
	if (ft_is_long(input) == false)
		return (false);
	while (input[i] != '\0')
	{
		a = input[i];
		if (i == 0 && (a == '+' || a == '-'))
		{
			++i;
			continue;
		}
		else if (ft_isdigit(a) == false)
			return (false);
		++i;
	}
	return (true);
}

/**
 * @author Prachi Chawda
*/
int	builtin_exit(int ac, char *av[])
{
	long	exit_number;

	if (isatty(STDIN_FILENO) == true && isatty(STDOUT_FILENO) == true)
		write(1, "exit\n", 5);
	if (ac > 2)
	{
		write(STDERR_FILENO, "minishell: exit: ", 17);
		write(STDERR_FILENO, "too many arguments\n", 19);
		return (1);
	}
	if (ac == 1)
		exit(0);
	if (is_number(av[1]) == false)
	{
		write(STDERR_FILENO, "minishell: exit: ", 17);
		write(STDERR_FILENO, av[1], ft_strlen(av[1]));
		write(STDERR_FILENO, ": numeric argument required\n", 28);
		exit(255);
	}
	exit_number = ft_atol(av[1]);
	if (exit_number < 0)
		exit_number = (exit_number % 256) + 256;
	if (exit_number >= 256)
		exit_number %= 256;
	exit(exit_number);
}
