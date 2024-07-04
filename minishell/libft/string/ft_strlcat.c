/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:48:14 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:05:23 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	end_dst;
	size_t	len_src;
	size_t	length;
	size_t	i;

	i = 0;
	end_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	if (size <= end_dst)
		return (size + len_src);
	length = size - end_dst - 1;
	while (i < length && src[i] != 0)
	{
		dst[end_dst + i] = src[i];
		i++;
	}
	dst[end_dst + i] = 0;
	return (end_dst + len_src);
}
/*
int main(void)
{
	char *str = "the cake is a lie !";
	char buff1[0xF00] = "there is no stars in the sky";
	size_t max = strlen("the cake is a lie !") + 4;
	size_t r1 = ft_strlcat(buff1, str, max);
	printf("%s\n%ld\n", buff1, r1);
}*/
