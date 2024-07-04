/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:14:56 by akernot           #+#    #+#             */
/*   Updated: 2023/10/26 14:26:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "get_next_line.h"

int	check_newline(char *string)
{
	int	i;

	i = 0;
	if (string == NULL)
		return (0);
	while (string[i] != '\0')
	{
		if (string[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	*gnl_calloc(int nmemb, int size)
{
	char	*ptr;
	int		i;

	ptr = (char *)malloc(nmemb * size);
	if (!ptr)
	{
		return (NULL);
	}
	i = 0;
	while (i < nmemb * size)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}

int	gnl_linelen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != 0)
	{
		if (s[i] == '\n')
			return (++i);
		i++;
	}
	return (i);
}

void	*gnl_memcpy(void *dest, const void *src, int n)
{
	char			*int_dest;
	const char		*int_src;
	int				i;

	i = 0;
	if (dest == NULL && src == NULL)
		return (NULL);
	int_dest = (char *)dest;
	int_src = (const char *)src;
	while (i < n)
	{
		int_dest[i] = int_src[i];
		i++;
	}
	return (dest);
}

/* Normal strjoin except it will stop at the first \n found */
char	*gnl_strjoin(char const *s1, char const *s2)
{
	char	*new_string;
	int		size1;
	int		size2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	size1 = gnl_linelen((char *)s1);
	size2 = gnl_linelen((char *)s2);
	new_string = (char *)gnl_calloc(size1 + size2 + 1, 1);
	if (!new_string)
		return (NULL);
	gnl_memcpy(new_string, s1, size1);
	if (check_newline(new_string))
		return (new_string);
	gnl_memcpy(&new_string[size1], s2, size2);
	return (new_string);
}
