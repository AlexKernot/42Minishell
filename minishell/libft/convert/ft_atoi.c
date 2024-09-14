/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 18:03:12 by akernot           #+#    #+#             */
/*   Updated: 2024/08/31 14:28:29 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

static int	skip_whitespace(const char *nptr)
{
	int	i;

	i = 0;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == ' ')
		i++;
	return (i);
}

static int	all_zeros(const char *nptr, int i)
{
	while (ft_isdigit(nptr[i]))
	{
		if (nptr[i] != 0)
			return (0);
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	long	converted;
	int		negative;
	int		i;

	converted = 0;
	negative = 0;
	i = skip_whitespace(nptr);
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		negative = 1;
		i++;
	}
	if (all_zeros(nptr, i))
		return (0);
	while (ft_isdigit(nptr[i]))
	{
		converted *= 10;
		converted += (long)nptr[i] - '0';
		i++;
	}
	if (negative == 1)
		converted *= -1;
	return ((int)converted);
}
/*
int main (void)
{
	printf("%d\n", ft_atoi("  100"));
	return (0);
}*/
