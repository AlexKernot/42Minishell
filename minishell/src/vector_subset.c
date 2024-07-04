/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_subset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 20:29:57 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:49:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "libft.h"

/**
 * @author Alex Kernot
 * @brief calculates the length of an array of strings.
 * @param array data to count length of.
 * @return length of array.
*/
int	ft_arrlen(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		++i;
	}
	return (i);
}

/**
 * @author Alex Kernot
 * @brief create a new character array that is a subset of the vector between
 * indicies start and end inclusively.
 * @param vec the vector to read from.
 * @param start the index in vec to start reading at.
 * @param end the index to stop reading at.
 * @warning this function will not copy the value at index end.
*/
char	**vector_subset(t_vector *vec, int start, int end)
{
	unsigned int	size;
	char			**array;
	int				i;

	if (vec->size < end)
		end = vec->size;
	if (end < start)
		return (NULL);
	size = end - start + 1;
	array = (char **)ft_calloc(sizeof(char *), size + 1);
	i = start;
	while (i <= end)
	{
		array[i - start] = vec->array[i];
		++i;
	}
	array[i] = 0;
	return (array);
}

/**
 * @author Alex Kernot
 * @brief create a new string array that is a subset of the string array between
 * indicies start and end inclusively.
 * @param input the string array to read from.
 * @param start the index in vec to start reading at.
 * @param end the index to stop reading at.
 * @return the new string array containing values from index start to end.
 * @warning this function will not copy the value at index end.
*/
char	**char_array_subset(char **input, int start, int end)
{
	char	**str;
	int		size;
	int		i;

	if (input == NULL)
		return (NULL);
	if (end < start)
		return (NULL);
	if (start == end)
		++end;
	size = end - start;
	str = ft_calloc(size + 1, sizeof(*str));
	if (str == NULL)
		return (NULL);
	i = start;
	while (i < end)
	{
		if (input[i] == NULL)
			return (str);
		str[i - start] = ft_strdup(input[i]);
		++i;
	}
	return (str);
}
