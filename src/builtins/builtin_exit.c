/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:54:08 by pchawda           #+#    #+#             */
/*   Updated: 2024/01/28 18:45:19 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "libft.h"
#include "builtin.h"

/**
 * @author Prachi Chawda
*/
int	builtin_exit(int ac, char *av[])
{
	write(1, "exit\n", 5);
	if (ac == 1)
		exit(0);
	if (ft_isdigit(av[1][0]) == true)
		exit(av[1][0] - '0');
	exit(av[1][0] % 256);
}
