/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:17:51 by akernot           #+#    #+#             */
/*   Updated: 2023/07/29 15:35:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stack.h"
#include <stdlib.h>

t_stack	*stack_ctor(int stack_size)
{
	t_stack	*new_stack;

	new_stack = (t_stack *)malloc(sizeof(t_stack));
	if (new_stack == NULL)
		return (NULL);
	new_stack->data = vector_ctor(stack_size);
	return (new_stack);
}

/* Frees the stack and vector inside the stack, setting the pointers to NULL. */
void	stack_dtor(t_stack **this_stack)
{
	t_stack	*stack;

	if (this_stack == NULL)
		return ;
	stack = *this_stack;
	if (stack == NULL)
		return ;
	if (stack->data != NULL)
		vector_dtor(&stack->data);
	free(*this_stack);
	*this_stack = NULL;
}

void	stack_push(t_stack *this_stack, void *data)
{
	t_vector	*vec;

	if (this_stack == NULL || this_stack->data == NULL
		|| this_stack->data->array == NULL)
		return ;
	vec = this_stack->data;
	vector_push_back(vec, data);
}

void	*stack_pop(t_stack *this_stack)
{
	t_vector	*vector;
	int			vector_size;
	void		*data;

	if (this_stack == NULL)
		return (NULL);
	vector = this_stack->data;
	if (vector == NULL || vector->array == NULL)
		return (NULL);
	vector_size = vector->size;
	if (vector_size == 0)
		return (NULL);
	data = vector_end(vector);
	(vector->size)--;
	return (data);
}

void	*stack_front(t_stack *this_stack)
{
	t_vector	*vector;

	if (this_stack == NULL)
		return (NULL);
	if (this_stack->data == NULL)
		return (NULL);
	if (this_stack->data->array == NULL || this_stack->data->size == 0)
		return (NULL);
	vector = this_stack->data;
	return (vector_end(vector));
}
