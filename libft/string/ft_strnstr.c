/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:12:32 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:05:38 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t		i;
	size_t		j;
	char		*match_start;

	i = 0;
	if (haystack == needle || *needle == 0)
		return ((char *)haystack);
	match_start = (char *)haystack;
	while (i < n && haystack[i] != 0)
	{
		if (haystack[i] == *needle)
		{
			j = 0;
			match_start = (char *)&haystack[i];
			while (haystack[i + j] == needle[j] && i + j < n)
			{
				if (needle[j + 1] == 0)
					return (match_start);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}
