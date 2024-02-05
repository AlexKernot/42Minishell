/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:56:31 by akernot           #+#    #+#             */
/*   Updated: 2023/12/03 22:08:34 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

t_bool	ft_putendl(const char *s)
{
	if (s == NULL)
		return (false);
	if (ft_putstr(s) == false)
		return (false);
	if (ft_putchar('\n') == false)
		return (false);
	return (true);
}
