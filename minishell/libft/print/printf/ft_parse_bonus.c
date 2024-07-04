/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 18:59:02 by akernot           #+#    #+#             */
/*   Updated: 2023/06/14 22:50:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "ft_printf.h"
#include "libft.h"
#include "ft_parse_format.h"
#include "ft_pointer.h"
#include "ft_string.h"
#include "ft_hexadecimal.h"
#include "ft_integer.h"

int	check_converter_bonus(const char letter)
{
	const char	converters[] = {'d', 'i', 's', 'c', 'u', 'x', 'X', 'p', '%'};
	const int	num_converters = 9;
	int			i;

	i = 0;
	while (i < num_converters)
	{
		if (letter == converters[i])
			return (TRUE);
		++i;
	}
	return (FALSE);
}

int	format_length_bonus(const char *format)
{
	int	i;

	i = 1;
	while (format[i] != '\0')
	{
		if (check_converter_bonus(format[i]) == TRUE)
			return (i);
		if (ft_isalpha(format[i]) == TRUE)
			return (-1);
		++i;
	}
	return (-1);
}

static void	parse_arguments(const char *format, t_switches *arguments)
{
	++format;
	while (check_converter_bonus(*format) == FALSE)
	{
		if (*format == '#')
			arguments->keep_zeros = 1;
		if (*format == '0' && arguments->min_width == -1)
			arguments->pad = '0';
		if (*format == ' ')
			arguments->space_or_plus = ' ';
		if (*format == '+')
			arguments->space_or_plus = '+';
		if (*format == '-')
			arguments->pad = -1;
		if (*format == '.')
		{
			if (format[1] == '-')
				break ;
			arguments->precision = ft_atoi(++format);
			break ;
		}
		if (*format != '0' && ft_isdigit(*format) && arguments->min_width == -1)
			arguments->min_width = ft_atoi(format);
		++format;
	}
	return ;
}

static int	converter_switch(const char converter,
								va_list *args, t_switches arguments)
{
	if (converter == '%')
		return (convert_percent(&arguments));
	if (converter == 'd' || converter == 'i')
		return (convert_int(va_arg(*args, int), &arguments));
	if (converter == 'x')
		return (convert_hex(va_arg(*args, unsigned int), 1, &arguments));
	if (converter == 'X')
		return (convert_hex(va_arg(*args, unsigned int), 0, &arguments));
	if (converter == 'p')
		return (convert_pointer(va_arg(*args, uint64_t), &arguments));
	if (converter == 'u')
		return (convert_unsigned(va_arg(*args, unsigned long), &arguments, 0));
	if (converter == 'c')
		return (convert_char(va_arg(*args, int), &arguments));
	if (converter == 's')
		return (convert_string(va_arg(*args, char *), arguments));
	return (-1);
}

int	process_converter_bonus(const char *format, va_list *args)
{
	int			print_length;
	int			converter_length;
	char		converter;
	t_switches	arguments;

	converter_length = format_length_bonus(format);
	if (converter_length < 1)
	{
		ft_printf("\x1b[1;31m!-- There was a problem with ");
		ft_printf("the format specified. --!");
		return (-1);
	}
	arguments.keep_zeros = 0;
	arguments.precision = -1;
	arguments.min_width = -1;
	arguments.pad = ' ';
	arguments.space_or_plus = 0;
	converter = format[converter_length];
	if (converter_length > 1)
		parse_arguments(format, &arguments);
	print_length = converter_switch(converter, args, arguments);
	return (print_length);
}
