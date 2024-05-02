/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:54:08 by pchawda           #+#    #+#             */
/*   Updated: 2024/05/02 17:07:36 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "libft.h"
#include "builtin.h"

t_bool is_number(char *input)
{
	int		i;
	char	a;

	i = 0;
	while (input[i] != '\0')
	{
		a = input[i];
		if (a != '+' && a != '-' && ft_isdigit(a) == false)
			return (false);
	}
	return (true);
}

/**
 * @author Prachi Chawda
*/
int	builtin_exit(int ac, char *av[])
{
	write(1, "exit\n", 5);
	if (ac > 2)
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments", 36);
		return (1);
	}
	if (ac == 1)
		exit(0);
	if (ft_isdigit(av[1][0]) == true)
		exit(av[1][0] - '0');
	exit(av[1][0] % 256);
}
