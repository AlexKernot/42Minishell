/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_integer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:42:02 by akernot           #+#    #+#             */
/*   Updated: 2023/12/03 22:09:10 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_integer.h"

#include "ft_printf.h"
#include "libft.h"
#include "ft_ultoa.h"
#include "ft_print_utils.h"

#include <unistd.h>
#include <stdlib.h>

static char	*handle_args(char *number, int is_negative,
							int strlen, t_switches *args)
{
	char		*temp_string;

	if (args->space_or_plus != 0 && is_negative == FALSE
		&& args->pad == '0' && args->precision == -1)
		--args->min_width;
	if (args->min_width > strlen && args->pad == '0' && args->precision == -1)
	{
		args->precision = args->min_width;
		args->min_width = -1;
	}
	if (args->precision > strlen)
		number = handle_precision(number, args->precision - strlen);
	temp_string = number;
	if (args->space_or_plus != 0 || is_negative == TRUE)
	{
		if (is_negative == TRUE)
			temp_string = ft_strjoin("-", number);
		else if (args->space_or_plus == ' ')
			temp_string = ft_strjoin(" ", number);
		else
			temp_string = ft_strjoin("+", number);
		free(number);
	}
	return (temp_string);
}

int	convert_int(long number, t_switches *args)
{
	int			add_one;
	int			len;
	int			is_negative;

	add_one = FALSE;
	is_negative = FALSE;
	if (number < 0)
	{
		is_negative = TRUE;
		number = -number;
	}
	if (is_negative == TRUE && args->pad == '0' && args->precision == -1)
	{
		ft_putchar_fd('-', 1);
		is_negative = FALSE;
		add_one = TRUE;
		--args->min_width;
		args->space_or_plus = 0;
	}
	len = convert_unsigned(number, args, is_negative);
	if (len == -1)
		return (-1);
	return (len + add_one);
}

int	convert_unsigned(uint64_t number, t_switches *args, int is_negative)
{
	char		*string;
	int			len;

	string = ft_ultoa(number);
	if (!string)
		return (-1);
	if (args->precision == 0 && number == 0)
	{
		free (string);
		string = (char *)ft_calloc(1, 1);
		if (!string)
			return (-1);
	}
	string = handle_args(string, is_negative, ft_strlen(string), args);
	if (!string)
		return (-1);
	if (args->precision != -1 && args->pad == '0')
		args->pad = ' ';
	args->precision = -1;
	len = print_number(string, args);
	free(string);
	return (len);
}
