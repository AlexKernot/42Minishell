/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexadecimal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:04:21 by akernot           #+#    #+#             */
/*   Updated: 2023/06/02 13:55:30 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEXADECIMAL_H
# define FT_HEXADECIMAL_H

# include <stdint.h>
# include "ft_printf.h"

int	convert_hex(uint64_t number, int lowercase, t_switches *options);
#endif
