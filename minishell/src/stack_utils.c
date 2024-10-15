/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:45:06 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:50:10 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shunting_yard.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "tokenizer.h"
#include "stack.h"
#include "syntax_tree.h"
#include "libft.h"

void	purge_stack(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack)
{
	if (tree == NULL || c_stack == NULL || o_stack == NULL)
		return ;
	if (c_stack->data->size == 1 && o_stack->data->size == 0)
	{
		create_dummy(tree, c_stack, ft_strdup("."));
		return ;
	}
	if (*tree == NULL && o_stack->data->size >= 1 && c_stack->data->size >= 2)
	{
		create_syntax_tree(tree, (char *)stack_pop(o_stack),
			(t_command *)stack_pop(c_stack),
			(t_command *)stack_pop(c_stack));
	}
	while (stack_front(o_stack) != NULL || stack_front(c_stack) != NULL)
	{
		add_op_to_syntax_tree(tree,
			(char *)stack_pop(o_stack),
			(t_command *)stack_pop(c_stack));
	}
}

int	initialize_stacks(t_stack **o_stack, t_stack **c_stack)
{
	if (o_stack == NULL || c_stack == NULL)
		return (1);
	*o_stack = stack_ctor(10);
	if (o_stack == NULL)
		return (1);
	*c_stack = stack_ctor(10);
	stack_push(*c_stack, create_command(NULL));
	if (c_stack == NULL)
	{
		stack_dtor(o_stack);
		return (1);
	}
	return (0);
}
