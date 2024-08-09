/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@42adel.org.au>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 14:32:32 by akernot           #+#    #+#             */
/*   Updated: 2023/07/26 16:42:10 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

#include <stdlib.h>

void	vector_set(t_vector *vector, int index, void *data)
{
	if (index < 0)
		return ;
	if (index > vector->capacity)
		return ;
	vector->array[index] = data;
}

void	*vector_get(t_vector *vector, int index)
{
	if (index > vector->capacity)
		return (0);
	if (index < 0)
		return (0);
	return (vector->array[index]);
}

void	*vector_front(t_vector *vector)
{
	return (vector->array[0]);
}

void	*vector_end(t_vector *vector)
{
	const int	index = vector->size - 1;

	if (index < 0)
		return (NULL);
	return (vector->array[index]);
}
