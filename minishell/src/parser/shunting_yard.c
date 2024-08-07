/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:25:37 by akernot           #+#    #+#             */
/*   Updated: 2024/08/06 16:51:11 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shunting_yard.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "tokenizer.h"
#include "stack.h"
#include "syntax_tree.h"
#include "libft.h"

int	is_operator(const char *token)
{
	if (token == NULL)
		return (0);
	if (token[0] == '(' && token[1] == '\0')
		return (1);
	if (token[0] == ')' && token[1] == '\0')
		return (1);
	if (token[0] == '|' && ((token[1] == '|' && token[2] == '\0')
		|| token[1] == '\0'))
		return (1);
	if (token[0] == '&' && token[1] == '&' && token[2] == '\0')
		return (1);
	return (0);
}

int	get_precedence(const char *token)
{
	if (token == NULL)
		return (0);
	if (token[0] == '(' && token[1] == '\0')
		return (10);
	if (token[0] == ')' && token[1] == '\0')
		return (10);
	if (token[0] == '|' && token[1] == '|' && token[2] == '\0')
		return (1);
	if (token[0] == '&' && token[1] == '&' && token[2] == '\0')
		return (1);
	if (token[0] == '|' && token[1] == '\0')
		return (2);
	return (0);
}

void	add_brackets(t_syntax_tree *tree, t_stack *c_stack,
			t_stack *o_stack, char *token)
{
	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	if (token[0] == '(' && token[1] == '\0') {
		stack_push(o_stack, token);
		return ;
	}
	else if (token[0] == ')' && token[1] == '\0')
	{
		while (stack_front(o_stack) != NULL
			&& ((char *)stack_front(o_stack))[0] != '(')
		{
			if (c_stack->data->size == 0)
				return ;
			add_op_to_syntax_tree(&tree, stack_pop(o_stack),
				stack_pop(c_stack));
		}
		if (stack_front(o_stack) != NULL
			&& ((char *)stack_front(o_stack))[0] == '(')
		{
			free(stack_pop(o_stack));
		}
	}
}

int		is_brackets(const char *token)
{
	if (token == NULL)
		return (0);
	if (token[0] == '(' && token[1] == '\0')
		return (1);
	if (token[0] == ')' && token[1] == '\0')
		return (1);
	return (0);
}

void	add_operator(t_syntax_tree *tree, t_stack *c_stack,
			t_stack *o_stack, char *token)
{
	const int	precedence = get_precedence(token);

	if (tree == NULL || c_stack == NULL ||  o_stack == NULL ||token == NULL)
		return ;
	if ((token[0] == '(' && token[1] == '\0')
		|| (token[0] == ')' && token[1] == '\0'))
		add_brackets(tree, c_stack, o_stack, token);
	else
	{
		while (get_precedence(stack_front(o_stack)) >= precedence
			&& o_stack->data->size != 0
			&& is_brackets(stack_front(o_stack)) == 0)
			add_op_to_syntax_tree(&tree, stack_pop(o_stack),
				stack_pop(c_stack));
		stack_push(o_stack, token);
	}
}

void	add_token(t_syntax_tree *tree, t_stack *c_stack, t_stack *o_stack,
		char *token)
{
	static t_command	*last_command = NULL;

	if (tree == NULL || c_stack == NULL || o_stack == NULL || token == NULL)
		return ;
	else if (is_operator(token))
	{
		if (tree == NULL)
			create_syntax_tree(&tree, token,
				stack_pop(c_stack), stack_pop(c_stack));
		else
			add_op_to_syntax_tree(&tree, token, stack_pop(c_stack));
		last_command = NULL;
	}
	else if (last_command == NULL)
	{
		last_command = create_command(token);
		stack_push(c_stack, last_command);
	}
	else
		push_token(last_command->args, token);
}

int	is_redirect(const char *token)
{
	if (token == NULL)
		return (0);
	if (token == NULL || token[0] == '\0')
		return (0);
	if (token[0] == '<' && token[1] == '\0')
		return (1);
	if (token[0] == '<' && token[1] == '<' && token[2] == '\0')
		return (1);
	if (token[0] == '>' && token[1] == '\0')
		return (1);
	if (token[0] == '>' && token[1] == '>' && token[2] == '\0')
		return (1);
	return (0);
}

void	purge_stack(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack)
{
	if (tree == NULL || c_stack == NULL || o_stack == NULL)
		return ;
	if (*tree == NULL && o_stack->data->size >= 1 && c_stack->data->size >= 2)
		create_syntax_tree(tree, (char *)stack_pop(o_stack),
			(t_command *)stack_pop(c_stack),
			(t_command *)stack_pop(c_stack));
	while (stack_front(o_stack) != NULL && stack_front(c_stack) != NULL)
	{
		add_op_to_syntax_tree(tree,
			(char *)stack_pop(o_stack),
			(t_command *)stack_pop(c_stack));
	}
}

static int	initialize_stacks(t_stack **o_stack, t_stack **c_stack)
{
	if (o_stack == NULL || c_stack == NULL)
		return (1);
	*o_stack = stack_ctor(10);
	if (o_stack == NULL)
		return (1);
	*c_stack = stack_ctor(10);
	if (c_stack == NULL)
	{
		stack_dtor(o_stack);
		return (1);
	}
	return (0);
}

t_syntax_tree	*convert_postfix(t_token_list *tokens)
{
	t_syntax_tree	*syntax_tree;
	t_stack		*o_stack;
	t_stack		*c_stack;
	char		*token;
	uint16_t	i;

	syntax_tree = NULL;
	if (initialize_stacks(&o_stack, &c_stack) == 1)
		return (NULL);
	i = 0;
	while (i < token_list_size(tokens))
	{
		token = get_token(tokens, i);
		if (is_redirect(token))
			add_redirect(stack_front(c_stack), token,
				get_token(tokens, i + 1));
		else
			add_token(syntax_tree, c_stack, o_stack, token);
		++i;
	}
	purge_stack(&syntax_tree, c_stack, o_stack);
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	return (syntax_tree);
}
