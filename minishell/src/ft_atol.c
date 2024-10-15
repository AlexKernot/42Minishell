/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 18:03:12 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 15:57:27 by akernot          ###   ########.fr       */
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
		++i;
	}
	return (1);
}

long	ft_atol(const char *nptr)
{
	int64_t	converted;
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
		converted += (int64_t)nptr[i] - '0';
		i++;
	}
	if (negative == 1)
		converted *= -1;
	return ((long)converted);
}

int	ft_is_long(const char *nptr)
{
	unsigned long	converted;
	size_t			negative;
	size_t			i;

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
	if (ft_strlen(&nptr[i]) > 19U + negative)
		return (false);
	while (ft_isdigit(nptr[i]))
	{
		converted *= 10;
		converted += (int64_t)nptr[i] - '0';
		i++;
	}
	if (negative == 1)
		return (converted < 9223372036854775809UL);
	return (converted < 9223372036854775808UL);
}

/*
int main (void)
{
	printf("%d\n", ft_atoi("  100"));
	return (0);
}*/
