/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_integer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:08:15 by akernot           #+#    #+#             */
/*   Updated: 2023/06/07 00:04:21 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_INTEGER_H
# define FT_INTEGER_H

# include "ft_printf.h"
# include <stdint.h>

int	convert_int(long number, t_switches *args);
int	convert_unsigned(uint64_t number, t_switches *args, int is_negative);
#endif
