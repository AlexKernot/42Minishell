/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:24:45 by akernot           #+#    #+#             */
/*   Updated: 2024/07/27 16:52:41 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

#include <stdlib.h>
#include <unistd.h>

t_vector	*vector_ctor(int capacity)
{
	t_vector		*new;
	void			**array;
	int				i;

	new = (t_vector *)malloc(sizeof(t_vector));
	array = malloc(sizeof(*(new->array)) * capacity);
	new->array = array;
	new->size = 0;
	new->capacity = capacity;
	i = 0;
	while (i < capacity)
	{
		array[i] = NULL;
		++i;
	}
	return (new);
}

void	vector_dtor(t_vector **vector)
{
	int	i;

	if (vector == 0 || *vector == 0)
		return ;
	if ((*vector)->array == 0)
		return ;
	i = 0;
	while (i < (*vector)->size)
	{
		if ((*vector)->array[i] != NULL)
		{
			free ((*vector)->array[i]);
			(*vector)->array[i] = NULL;
		}
		++i;
	}
	free((*vector)->array);
	free(*vector);
	*vector = NULL;
}

void	vector_resize(t_vector *vector, int size)
{
	void		**new_array;
	int			i;

	new_array = malloc(sizeof(*(vector->array)) * size);
	i = 0;
	if (vector->size > size)
		vector->size = size;
	while (i < vector->capacity && i < size)
	{
		new_array[i] = vector->array[i];
		++i;
	}
	vector->capacity = size;
	free(vector->array);
	vector->array = new_array;
}

void	vector_push_back(t_vector *vector, void *data)
{
	if (vector->size + 1 == vector->capacity)
	{
		vector_resize(vector, vector->capacity * 2);
		if (vector->array == NULL)
		{
			write(2, "Vector resize failed.\n", 22);
			return ;
		}
	}
	vector->array[vector->size] = data;
	vector->size += 1;
}

void	vector_pop_back(t_vector *vector)
{
	const int	index = vector->size - 1;
	void		*data;

	if (vector->size == 0)
		return ;
	data = vector->array[index];
	vector->array[index] = NULL;
	vector->size--;
	free(data);
}
