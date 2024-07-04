/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 14:42:23 by akernot           #+#    #+#             */
/*   Updated: 2023/06/07 09:56:23 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_parse_format.h"
#include "ft_hexadecimal.h"
#include "ft_printf.h"
#include "ft_pointer.h"

int	convert_pointer(unsigned long int number, t_switches *arguments)
{
	int	len;

	arguments->keep_zeros = 2;
	len = convert_hex(number, 1, arguments);
	return (len);
}
