/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:53:44 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:23:02 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strndup.h"

#include "stddef.h"
#include "stdlib.h"

#include "libft.h"

char	*ft_strndup(const char *str, const size_t n)
{
	char	*new_string;
	size_t	i;
	size_t	size;

	i = 0;
	if (*str == 0)
		return (ft_calloc(1, 1));
	size = ft_strlen(str);
	new_string = (char *)malloc(size * sizeof(char) + 1);
	if (new_string == NULL || size < 1)
		return (NULL);
	while (i < size && str[i] != 0 && i < n)
	{
		new_string[i] = str[i];
		i++;
	}
	new_string[i] = 0;
	return (new_string);
}
