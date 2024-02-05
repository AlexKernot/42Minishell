/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 19:00:08 by akernot           #+#    #+#             */
/*   Updated: 2023/06/14 22:50:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

#include <stdarg.h>
#include <unistd.h>

#include "libft.h"
#include "ft_parse_bonus.h"

int	jump(const char *format)
{
	int	i;

	i = 1;
	while (format[i] != '\0')
	{
		if (check_converter_bonus(format[i]))
			return (i);
		++i;
	}
	return (0);
}

int	handle_format(const char *format, va_list *args)
{
	int	print_length;
	int	current_length;

	print_length = 0;
	while (*format != '\0')
	{
		if (*format != '%')
		{
			if (write(1, format, 1) == -1)
				return (-1);
			++print_length;
			++format;
			continue ;
		}
		current_length = process_converter_bonus(format, args);
		if (current_length == -1)
			return (-1);
		print_length += current_length;
		format += jump(format) + 1;
	}
	return (print_length);
}

// %[$][flags][width][.precision][length modifier]conversion
int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		print_length;

	va_start(args, format);
	print_length = handle_format(format, &args);
	va_end(args);
	return (print_length);
}
