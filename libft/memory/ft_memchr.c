/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 14:46:13 by akernot           #+#    #+#             */
/*   Updated: 2023/10/26 17:40:42 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	to_find;
	unsigned char	*array;
	void			*found;
	size_t			i;

	i = 0;
	to_find = (unsigned char)c;
	array = (unsigned char *)s;
	while (i < n)
	{
		if (array[i] == to_find)
		{
			found = (void *)&array[i];
			return (found);
		}
		i++;
	}
	return (NULL);
}
