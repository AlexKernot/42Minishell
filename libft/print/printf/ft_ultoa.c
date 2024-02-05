/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 13:28:37 by akernot           #+#    #+#             */
/*   Updated: 2023/06/14 22:50:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "libft.h"

static int	count_digits(unsigned int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

static void	convert(unsigned int number, char **str, int pos)
{
	if (number >= 10)
		convert(number / 10, str, pos - 1);
	str[0][pos] = (number % 10) + '0';
}

char	*ft_ultoa(unsigned int n)
{
	int				length;
	unsigned int	working_number;
	char			*string;

	if (n == 0)
		return (ft_strdup("0"));
	length = count_digits(n);
	string = (char *)ft_calloc(length + 1, sizeof(char));
	if (!string)
		return (NULL);
	working_number = n;
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
