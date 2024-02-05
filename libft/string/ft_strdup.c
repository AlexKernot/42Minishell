/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:13:34 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:05:10 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*new_string;
	size_t	i;
	size_t	size;

	i = 0;
	if (*s == 0)
		return (ft_calloc(1, 1));
	size = ft_strlen(s);
	new_string = (char *)malloc(size * sizeof(char) + 1);
	if (new_string == NULL || size < 1)
		return (NULL);
	while (i < size && s[i] != 0)
	{
		new_string[i] = s[i];
		i++;
	}
	new_string[i] = 0;
	return (new_string);
}
