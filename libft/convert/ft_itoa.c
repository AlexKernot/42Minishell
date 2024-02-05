/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:28:37 by akernot           #+#    #+#             */
/*   Updated: 2023/10/26 14:45:12 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "libft.h"

static int	count_digits(long n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		i++;
		n = -n;
	}
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

void	convert(long number, char **str, int pos)
{
	if (number < 0)
	{
		str[0][0] = '-';
		number = -number;
	}
	if (number >= 10)
		convert(number / 10, str, pos - 1);
	str[0][pos] = (number % 10) + '0';
}

char	*ft_itoa(int n)
{
	int		length;
	long	working_number;
	char	*string;

	length = count_digits((long)n);
	string = (char *)ft_calloc(length + 1, sizeof(char));
	if (!string)
		return (NULL);
	if (n == 0)
	{
		string[0] = '0';
		string[1] = '\0';
		return (string);
	}
	working_number = (long)n;
	convert(working_number, &string, length - 1);
	string[length] = 0;
	return (string);
}
/*
int	main(void)
{
	char	*string3 = ft_itoa(-5859);

	ft_putendl_fd(string3, 1);
	free(string3);
	return (0);
}*/
