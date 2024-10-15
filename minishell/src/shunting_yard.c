/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:25:37 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:20:29 by akernot          ###   ########.fr       */
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

void	create_dummy(t_syntax_tree **tree, t_stack *c_stack, char *token)
{
	t_syntax_tree	*node;

	node = (t_syntax_tree *)malloc(sizeof(*node));
	node->contents.type = op;
	node->contents.contents.operator_word = token;
	if (*tree == NULL)
	{
		*tree = node;
		node->right = NULL;
		node->left = (t_syntax_tree *)malloc(sizeof(*node->left));
		node->left->left = NULL;
		node->left->right = NULL;
		node->left->contents.type = command;
		node->left->contents.contents.command = stack_pop(c_stack);
		return ;
	}
	node->left = *tree;
	*tree = node;
	node->right = (t_syntax_tree *)malloc(sizeof(*node->right));
	node->right->left = NULL;
	node->right->right = NULL;
	node->right->contents.type = command;
	node->right->contents.contents.command = stack_pop(c_stack);
}

static void	append_syntax_tree(t_syntax_tree **tree, t_stack *c_stack,
	t_stack *o_stack, char *token)
{
	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	if (o_stack->data->size == 0)
	{
		create_dummy(tree, c_stack, token);
		return ;
	}
	else if (*tree == NULL)
	{
		create_syntax_tree(tree, stack_pop(o_stack),
			stack_pop(c_stack), stack_pop(c_stack));
		return ;
	}
	add_op_to_syntax_tree(tree, stack_pop(o_stack),
		stack_pop(c_stack));
}

void	add_brackets(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack, char *token)
{
	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	if (token[0] == '(' && token[1] == '\0')
	{
		stack_push(o_stack, token);
		return ;
	}
	else if (token[0] == ')' && token[1] == '\0')
	{
		while (stack_front(o_stack) != NULL
			&& ((char *)stack_front(o_stack))[0] != '(')
		{
			append_syntax_tree(tree, c_stack, o_stack, token);
		}
		if (stack_front(o_stack) != NULL
			&& ((char *)stack_front(o_stack))[0] == '(')
		{
			free(stack_pop(o_stack));
		}
	}
}

void	add_operator(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack, char *token)
{
	const int	precedence = get_precedence(token);

	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	if (is_brackets(token) == 1)
		add_brackets(tree, c_stack, o_stack, token);
	else
	{
		while (get_precedence(stack_front(o_stack)) >= precedence
			&& o_stack->data->size != 0
			&& is_brackets(stack_front(o_stack)) == 0)
			append_syntax_tree(tree, c_stack, o_stack, token);
		stack_push(o_stack, token);
	}
}

void	add_token(t_syntax_tree **tree, t_stack *c_stack, t_stack *o_stack,
		char *token)
{
	static t_command	*last_command = NULL;

	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	if (stack_front(c_stack) == NULL)
	{
		stack_push(c_stack, create_command(NULL));
	}
	if (*tree == NULL && c_stack->data->size == 1 && o_stack->data->size == 0)
		last_command = NULL;
	if (last_command == NULL)
		last_command = stack_front(c_stack);
	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	if (is_operator(token))
	{
		add_operator(tree, c_stack, o_stack, token);
		last_command = create_command(NULL);
		stack_push(c_stack, last_command);
		return ;
	}
	if (last_command->command == NULL)
		last_command->command = ft_strdup(token);
	push_token(last_command->args, token);
}
