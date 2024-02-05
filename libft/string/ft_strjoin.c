/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 21:02:58 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:05:19 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_string;
	int		size1;
	int		size2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	size1 = ft_strlen((char *)s1);
	size2 = ft_strlen((char *)s2);
	new_string = (char *)malloc(sizeof(char) * (size1 + size2 + 1));
	if (!new_string)
		return (NULL);
	ft_memcpy((void *)new_string, (void *)s1, size1);
	ft_memcpy((void *)(&new_string[size1]), (void *)s2, size2);
	new_string[size1 + size2] = 0;
	return (new_string);
}
