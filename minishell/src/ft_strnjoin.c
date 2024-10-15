/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:52:36 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:54:28 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strnjoin.h"

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft.h"

char	*ft_strnjoin(const char *str1, const char *str2, int max_size)
{
	char	*str;
	int		size1;
	int		size2;

	size1 = ft_strlen(str1);
	size2 = ft_strlen(str2);
	if (size2 > max_size)
		size2 = max_size;
	str = (char *)malloc(sizeof(*str) * (size1 + size2 + 1));
	if (str == NULL)
		return (NULL);
	if (ft_memcpy(str, str1, size1) == NULL)
	{
		free(str);
		assert(0);
		return (NULL);
	}
	if (ft_memcpy(&str[size1], str2, size2) == NULL)
	{
		free(str);
		assert(0);
		return (NULL);
	}
	str[size1 + size2] = '\0';
	return (str);
}
