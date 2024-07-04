/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:41:16 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:03:23 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	ft_bzero(void *s, size_t n)
{
	unsigned char		*pointer;
	size_t				i;

	pointer = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		pointer[i] = 0;
		i++;
	}
}
