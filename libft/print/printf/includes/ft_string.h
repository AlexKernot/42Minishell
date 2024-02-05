/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42.ft>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 16:03:26 by akernot           #+#    #+#             */
/*   Updated: 2023/06/06 19:02:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRING_H
# define FT_STRING_H

int	convert_string(char *string, t_switches arguments);
int	convert_char(char letter, t_switches *arguments);
int	convert_percent(t_switches *args);
#endif
