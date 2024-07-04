/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexadecimal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 15:47:09 by akernot           #+#    #+#             */
/*   Updated: 2023/06/14 22:50:03 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include <stdint.h>

#include "libft.h"
#include "ft_print_utils.h"
#include "ft_printf.h"
#include "ft_hexadecimal.h"

static char	*handle_args(char *number, int is_negative,
							int strlen, t_switches *args)
{
	char		*temp_string;

	if (args->keep_zeros > 0 && args->min_width > strlen && args->precision < 0)
		strlen += 2;
	if (args->min_width > strlen && args->pad == '0' && args->precision < 0)
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
			temp_string = ft_strjoin("+", number);
		else
			temp_string = ft_strjoin(" ", number);
		free(number);
	}
	return (temp_string);
}

static int	number_length(uint64_t number)
{
	int	i;

	i = 0;
	while (number > 0)
	{
		++i;
		number /= 16;
	}
	return (i);
}

static char	*dec_to_hex(uint64_t number, int lower, t_switches *format)
{
	uint64_t		remainder;
	int				length;
	char			*new_string;

	if (format->precision == 0 && number == 0)
		return ((char *)ft_calloc(1, 1));
	if (number == 0)
		return (ft_strdup("0"));
	length = number_length(number);
	new_string = ft_calloc(1, length + 1);
	while (number > 0)
	{
		remainder = number % 16;
		if (remainder <= 9)
			new_string[length - 1] = remainder + '0';
		else
			new_string[length - 1] = remainder - 10 + 65 + (32 * lower);
		number /= 16;
		--length;
	}
	return (new_string);
}

int	convert_hex(uint64_t number, int lowercase, t_switches *options)
{
	int			len;
	char		*conversion;
	char		*full_string;

	conversion = dec_to_hex(number, lowercase, options);
	if (number == 0 && options->keep_zeros != 2)
		options->keep_zeros = 0;
	conversion = handle_args(conversion, FALSE, ft_strlen(conversion), options);
	if (options->keep_zeros > 0)
	{
		if (lowercase == TRUE)
			full_string = ft_strjoin("0x", conversion);
		else
			full_string = ft_strjoin("0X", conversion);
		free (conversion);
		conversion = full_string;
	}
	if (options->precision != -1 && options->pad == '0')
		options->pad = ' ';
	options->precision = -1;
	len = print_number(conversion, options);
	free(conversion);
	return (len);
}
