/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_format.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 14:58:22 by akernot           #+#    #+#             */
/*   Updated: 2023/06/02 13:56:11 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PARSE_FORMAT_H
# define FT_PARSE_FORMAT_H

# include <stdarg.h>

int	check_converter(const char letter);
int	format_length(const char *format);
int	process_converter(const char *format, va_list *args);
#endif
