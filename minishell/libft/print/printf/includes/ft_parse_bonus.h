/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:51:45 by akernot           #+#    #+#             */
/*   Updated: 2023/06/07 14:52:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PARSE_BONUS_H
# define FT_PARSE_BONUS_H

# include <stdarg.h>

int	check_converter_bonus(const char letter);
int	format_length_bonus(const char *format);
int	process_converter_bonus(const char *format, va_list *args);
#endif
