/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 20:39:04 by akernot           #+#    #+#             */
/*   Updated: 2023/06/06 23:57:31 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINT_UTILS_H
# define FT_PRINT_UTILS_H

# include "ft_printf.h"

char	*handle_precision(char *number, int precision);
int		print_string(char *string, int is_char, t_switches *args);
int		print_number(char *string, t_switches *args);
#endif
