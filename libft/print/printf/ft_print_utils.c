/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 20:04:23 by akernot           #+#    #+#             */
/*   Updated: 2023/06/14 22:50:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_print_utils.h"

#include "ft_printf.h"
#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

char	*handle_precision(char *number, int precision)
{
	char	*temp_string;
	char	*temp_string_2;

	temp_string_2 = (char *)ft_calloc(precision + 1, 1);
	(void)ft_memset(temp_string_2, '0', precision);
	temp_string = ft_strjoin(temp_string_2, number);
	free(temp_string_2);
	free(number);
	return (temp_string);
}

static int	put_chars(char padding, int number)
{
	int	i;

	i = 0;
	while (i < number)
	{
		if (write(1, &padding, 1) == -1)
			return (-1);
		++i;
	}
	return (0);
}

static int	put_str_width(char *string, int width, int is_char)
{
	int	i;

	i = 0;
	if (is_char)
	{
		if (write(1, string, 1) == -1)
			return (-1);
		return (0);
	}
	while ((i < width || width == -1) && string[i] != '\0')
	{
		if (write(1, &string[i], 1) == -1)
			return (-1);
		++i;
	}
	return (0);
}

int	print_string(char *string, int is_char, t_switches *args)
{
	int	len;

	len = ft_strlen(string);
	if (is_char == TRUE && *string == '\0')
		++len;
	if (args->min_width < len && args->precision == -1)
	{
		put_str_width(string, -1, is_char);
		return (len);
	}
	if (args->precision != -1 && len > args->precision)
		len = args->precision;
	if (args->pad == ' ')
	{
		put_chars(' ', args->min_width - len);
		put_str_width(string, args->precision, is_char);
		if (len < args->min_width)
			len += args->min_width - len;
		return (len);
	}
	put_str_width(string, args->precision, is_char);
	put_chars(' ', args->min_width - len);
	if (len < args->min_width)
		len += args->min_width - len;
	return (len);
}

int	print_number(char *string, t_switches *args)
{
	int	len;

	len = ft_strlen(string);
	if (args->min_width < len && args->precision == -1)
	{
		put_str_width(string, -1, FALSE);
		return (len);
	}
	if (args->precision != -1)
		args->min_width -= args->precision;
	args->min_width -= len;
	if (args->min_width > 0 && args->pad != -1)
		put_chars(args->pad, args->min_width);
	if (args->precision > len)
	{
		put_chars('0', args->precision - len);
		len = args->precision;
	}
	put_str_width(string, -1, FALSE);
	if (args->min_width > 0 && args->pad == -1)
		if (put_chars(' ', args->min_width) == -1)
			return (-1);
	if (args->min_width > 0)
		return (len + args->min_width);
	return (len);
}
