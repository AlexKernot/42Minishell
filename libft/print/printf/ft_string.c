/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 15:52:21 by akernot           #+#    #+#             */
/*   Updated: 2023/06/14 22:50:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ft_printf.h"
#include "ft_print_utils.h"
#include "libft.h"

int	convert_string(char *string, t_switches arguments)
{
	const char	*null_char = "(null)";
	int			len;

	if (string == NULL)
		string = (char *)null_char;
	len = print_string(string, FALSE, &arguments);
	return (len);
}

int	convert_char(char letter, t_switches *arguments)
{
	char	*string;
	int		len;

	string = ft_calloc(2, 1);
	if (!string)
		return (-1);
	string[0] = letter;
	arguments->precision = -1;
	len = print_string(string, TRUE, arguments);
	free(string);
	return (len);
}

int	convert_percent(t_switches *args)
{
	const char	*percent = "%";
	int			len;

	args->precision = -1;
	len = print_string((char *)percent, TRUE, args);
	return (len);
}
