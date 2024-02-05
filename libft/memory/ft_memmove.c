/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 14:33:12 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:04:45 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

// Memmove copys n bytes of src into a temporary array then again into dest.
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	const unsigned char	*char_src;
	unsigned char		*char_dest;

	char_src = (const unsigned char *)src;
	char_dest = (unsigned char *)dest;
	if (dest == src)
		return (dest);
	if (char_src < char_dest)
	{
		i = n;
		while (i > 0)
		{
			char_dest[i - 1] = char_src[i - 1];
			i--;
		}
		return (dest);
	}
	i = 0;
	while (i < n)
	{
		char_dest[i] = char_src[i];
		i++;
	}
	return (dest);
}
/*
int main(void)
{
	char *src = "this is a good nyancat !\r\n";
	char dst2[0xF0];
	int size = strlen(src);

	ft_memmove(dst2, src, size);
	printf("%s\n", dst2);
	return (0);
}*/
