/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 13:28:29 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:04:48 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char		*pointer;
	size_t				i;

	i = 0;
	pointer = (unsigned char *)s;
	while (i < n)
	{
		pointer[i] = c;
		i++;
	}
	return (s);
}
/*
int main(void)
{
	void *string = malloc(15 * sizeof(unsigned char));
	memset(string, '\0', 15);
	ft_memset(string, 'A', 10);

	printf("%s\n", (unsigned char *)string);
}*/
