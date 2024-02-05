/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 19:27:23 by akernot           #+#    #+#             */
/*   Updated: 2023/03/11 23:33:07 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

static int	check_seperator(const char c, const char *sep)
{
	int	i;

	i = 0;
	while (sep[i] != 0)
	{
		if (c == sep[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s, const char *sep)
{
	char	*new_string;
	int		length;
	int		start;
	int		end;

	if (s == NULL)
		return (NULL);
	length = ft_strlen((char *)s);
	start = 0;
	while (check_seperator(s[start], sep))
		start++;
	end = length - 1;
	while (check_seperator(s[end], sep) && end != start - 1)
		end--;
	length = end - start + 1;
	new_string = (char *)malloc(length * sizeof(char) + 1);
	if (!new_string)
		return (NULL);
	ft_memcpy((void *) new_string, (void *)(&s[start]), length);
	new_string[length] = 0;
	return (new_string);
}
/*
int	main(void)
{
	char *s1 = "  \t \t \n   \n\n\n\t";
	char *s2 = "";
	char *ret = ft_strtrim(s1, " \n\t");
	printf("%s\n", ret);
}*/
