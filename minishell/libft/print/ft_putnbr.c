/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:59:03 by akernot           #+#    #+#             */
/*   Updated: 2023/12/03 22:08:39 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_bool	int_to_char(long number, char **str, int pos)
{
	if (number < 0)
	{
		str[0][0] = '-';
		number = -number;
		pos++;
	}
	if (number >= 10)
		int_to_char(number / 10, str, pos - 1);
	if (ft_putchar((number) % 10 + '0') == false)
		return (false);
	str[0][pos] = (number % 10) + '0';
	return (true);
}

t_bool	ft_putnbr(int n)
{
	char	*num_point;
	char	number[12];
	int		copy;
	int		length;

	if (n == 0)
	{
		return (ft_putchar('0'));
	}
	copy = n;
	length = 0;
	if (copy < 0)
		copy = -copy;
	while (copy)
	{
		length++;
		copy /= 10;
	}
	num_point = &number[0];
	if (int_to_char((long)n, &num_point, length - 1))
		return (false);
	if (ft_putstr(num_point))
		return (false);
	return (true);
}
