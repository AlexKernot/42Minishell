/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:56:15 by akernot           #+#    #+#             */
/*   Updated: 2024/07/21 16:58:34 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H

/* Vector */

typedef struct s_vector
{
	void	**array;
	int		capacity;
	int		size;
}	t_vector;

/* Creates a vector with a pointer to an array of ints of size capacity       */
t_vector	*vector_ctor(int capacity);

/* Frees the entire array and node. Setting the pointer to NULL.              */
void		vector_dtor(t_vector **vector);

/* Adds an integer to the end of the int array, resizing if nessecary.        */
/* This function returns the new size of the array. */
void		vector_push_back(t_vector *vector, void *data);

/* Removes the last index from the array.                                     */
void		vector_pop_back(t_vector *vector);

/* Resizes the vector to size, copying the data from the old array into the   */
/* new one. This will only copy size bytes of data into the new array,        */
/* deleting all extra bytes.                                                  */
void		vector_resize(t_vector *vector, int size);

/* Inserts data at index. This function will overwrite data.                  */
void		vector_set(t_vector *vector, int index, void *data);

/* Returns a pointer to the t_packet at index, or NULL if failed.             */
void		*vector_get(t_vector *vector, int index);

/* Returns a pointer to the value at the end of the vector.                   */
void		*vector_end(t_vector *vector);

/* Returns a pointer to the value at the front of the vector. */
void		*vector_front(t_vector *vector);

/* Stack */
/* A last in first out data structure that creates a vector of            */
/* integer pointers. the output of stack_pop() needs to be freed.         */
/* On destruction, stack_dtor() will clean up all data held by the stack. */
typedef struct s_stack
{
	t_vector	*data;
}	t_stack;

/* Creates a new stack on the heap and returns a pointer to it. */
t_stack		*stack_ctor(int stack_size);

/* Frees the stack, and and all contained data. This function invalidates */
/* all pointers to this stack and inside data. */
/* It also sets the passed in pointer to stack to NULL. */
void		stack_dtor(t_stack **this_stack);

/* Adds the given void * to the top of the stack */
void		stack_push(t_stack *this_stack, void *data);

/* Removes the number on the top of the stack and returns it. */
void		*stack_pop(t_stack *this_stack);

/* Returns the value at the top of the stack. */
/* This function may return NULL. */
void		*stack_front(t_stack *this_stack);

#endif
