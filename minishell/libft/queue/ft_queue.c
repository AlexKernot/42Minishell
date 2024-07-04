/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_queue.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:17:51 by akernot           #+#    #+#             */
/*   Updated: 2023/12/03 22:07:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/* Allocates memory for a new queue on the heap with */
/* with an initial capacity of 'queue_size' */
t_queue	*queue_ctor(int queue_size)
{
	t_queue	*new_queue;

	new_queue = (t_queue *)malloc(sizeof(t_queue));
	if (new_queue == NULL)
		return (NULL);
	new_queue->data = vector_ctor(queue_size);
	new_queue->head = 0;
	return (new_queue);
}

/* Frees the queue and vector inside the queue, setting the pointers to NULL. */
void	queue_dtor(t_queue **this_queue)
{
	t_queue	*queue;

	if (this_queue == NULL)
		return ;
	queue = *this_queue;
	if (queue == NULL)
		return ;
	if (queue->data != NULL)
		vector_dtor(&queue->data);
	free(*this_queue);
	*this_queue = NULL;
}

/* Pushes the pointer 'data' onto the end of the queue. */
/* This is typically known as an enqueue operation. */
void	queue_push(t_queue *this_queue, void *data)
{
	t_vector	*vec;

	if (this_queue == NULL || this_queue->data == NULL
		|| this_queue->data->array == NULL)
		return ;
	vec = this_queue->data;
	vector_push_back(vec, data);
}

/* Removes the data at the front of the queue and returns it. */
/* This is commonly known as a dequeue operation. */
void	*queue_pop(t_queue *this_queue)
{
	t_vector	*vector;
	int			vector_size;
	void		*data;

	if (this_queue == NULL)
		return (NULL);
	vector = this_queue->data;
	if (vector == NULL || vector->array == NULL)
		return (NULL);
	vector_size = vector->size;
	if (vector_size == 0)
		return (NULL);
	data = vector_get(vector, this_queue->head);
	vector_set(vector, this_queue->head, 0);
	++this_queue->head;
	return (data);
}

/* Returns the data at the front of the queue. */
void	*queue_front(t_queue *this_queue)
{
	t_vector	*vector;

	if (this_queue == NULL)
		return (NULL);
	if (this_queue->data == NULL)
		return (NULL);
	if (this_queue->data->array == NULL || this_queue->data->size == 0)
		return (NULL);
	vector = this_queue->data;
	return (vector_get(vector, this_queue->head));
}
