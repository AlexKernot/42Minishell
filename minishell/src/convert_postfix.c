/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_postfix.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:31:48 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:33:07 by akernot          ###   ########.fr       */
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

static t_syntax_tree	*run_postfix_algorithm(t_token_list *tokens,
	t_stack *c_stack,
	t_stack *o_stack)
{
	t_syntax_tree	*syntax_tree;
	uint16_t		i;
	char			*token;

	i = 0;
	syntax_tree = NULL;
	while (i < token_list_size(tokens))
	{
		token = get_token(tokens, i);
		if (is_redirect(token))
		{
			add_redirect(stack_front(c_stack), token,
				get_token(tokens, i + 1), &i);
		}
		else
			add_token(&syntax_tree, c_stack, o_stack, token);
		++i;
	}
	return (syntax_tree);
}

static void	cleanup_postfix(t_syntax_tree **tree, t_stack **c_stack,
				t_stack **o_stack)
{
	delete_syntax_tree(tree);
	stack_dtor(c_stack);
	stack_dtor(o_stack);
}

t_syntax_tree	*convert_postfix(t_token_list *tokens)
{
	t_syntax_tree	*syntax_tree;
	t_command		*front;
	t_stack			*o_stack;
	t_stack			*c_stack;

	if (initialize_stacks(&o_stack, &c_stack) == 1)
		return (NULL);
	syntax_tree = run_postfix_algorithm(tokens, c_stack, o_stack);
	front = stack_front(c_stack);
	if (front != NULL && front->command == NULL)
	{
		front = stack_pop(c_stack);
		if (front->args->size != 0 || front->redirects->size != 0)
		{
			write(STDERR_FILENO, "minishell: syntax error\n", 24);
			cleanup_postfix(&syntax_tree, &c_stack, &o_stack);
			return (NULL);
		}
		delete_command(front);
		free(front);
	}
	purge_stack(&syntax_tree, c_stack, o_stack);
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	return (syntax_tree);
}
