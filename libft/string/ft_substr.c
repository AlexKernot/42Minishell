/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 18:15:12 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:05:49 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	str_length;
	char	*new_string;

	i = 0;
	if (s == NULL || *s == 0)
		return ((char *)ft_calloc(1, 1));
	str_length = ft_strlen(s);
	if (start > str_length)
		return (ft_calloc(1, 1));
	str_length -= start;
	if (len < str_length)
		str_length = len;
	new_string = (char *)ft_calloc(str_length + 1, sizeof(char));
	if (!new_string)
		return (NULL);
	while (i < str_length && s[start + i] != 0)
	{
		new_string[i] = s[start + i];
		i++;
	}
	return (new_string);
}
/*
int main(void)
{
	char *str = "01234";
	size_t size = 10;
	char *ret = ft_substr(str, 10, size);
	printf("%s\n", ret);
	free(ret);
}*/
