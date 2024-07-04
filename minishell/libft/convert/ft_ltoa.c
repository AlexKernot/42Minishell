/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 14:36:22 by akernot           #+#    #+#             */
/*   Updated: 2023/10/26 17:23:06 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "libft.h"

/* 123456789 */
/* 12345678 */
/* 1234567 */
/* 123456 */
/* ... */
/* 1 */

void	convert(unsigned long number, char **str, const int pos)
{
	if (number >= 10)
		convert(number / 10, str, pos - 1);
	(*str)[sizeof(number) - pos] = (number % 10) + '0';
}

char	*ft_ltoa(const long n)
{
	const unsigned int	size = sizeof(n) * 5;
	unsigned long		working_number;
	char				*string;

	string = (char *)ft_calloc(size + 1, sizeof(char));
	if (string == NULL)
		return (NULL);
	if (n == 0)
	{
		string[0] = '0';
		return (string);
	}
	working_number = (unsigned long)n;
	if (n < 0)
	{
		string[0] = '-';
		working_number = -n;
	}
	convert(working_number, &string, sizeof(n));
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
