/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:25:37 by akernot           #+#    #+#             */
/*   Updated: 2024/07/25 17:08:37 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shunting_yard.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "tokenizer.h"
#include "command_queue.h"
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

void	add_brackets(t_command_queue *queue, t_stack *stack, char *token)
{
	if (queue == NULL || stack == NULL || token == NULL)
		return ;
	if (token[0] == '(' && token[1] == '\0') {
		stack_push(stack, token);
		return ;
	}
	else if (token[0] == ')' && token[1] == '\0')
	{
		while (stack_front(stack) != NULL
			&& ((char *)stack_front(stack))[0] != '(')
		{
			command_enqueue_op(queue, stack_pop(stack));
		}
		if (stack_front(stack) != NULL
			&& ((char *)stack_front(stack))[0] == '(')
		{
			free(stack_pop(stack));
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

void	add_operator(t_command_queue *queue, t_stack *stack, char *token)
{
	const int	precedence = get_precedence(token);

	if (queue == NULL || stack == NULL || token == NULL)
		return ;
	if ((token[0] == '(' && token[1] == '\0')
		|| (token[0] == ')' && token[1] == '\0'))
		add_brackets(queue, stack, token);
	else
	{
		while (get_precedence(stack_front(stack)) >= precedence
			&& stack->data->size != 0
			&& is_brackets(stack_front(stack)) == 0)
			command_enqueue_op(queue, stack_pop(stack));
		stack_push(stack, token);
	}
}

void	add_redirect(t_command *command, char *redir, char *file)
{
	char	*redir_type;

	if (redir == NULL || command == NULL || file == NULL)
		return ;
	redir_type = (char *)malloc(2);
	if (redir_type == NULL)
		return ;
	redir_type[1] = '\0';
	if (redir[0] == '<' && redir[1] == '\0')
		redir_type[0] = '1';
	else if (redir[0] == '<' && redir[1] == '<' && redir[2] == '\0')
		redir_type[0] = '3';
	else if (redir[0] == '>' && redir[1] == '\0')
		redir_type[0] = '2';
	else if (redir[0] == '>' && redir[1] == '>' && redir[2] == '\0')
		redir_type[0] = '4';
	else
		redir_type[0] = '5';
	push_token(command->redir_types, redir_type);
	push_token(command->redirects, file);
}

void	add_token(t_command_queue *queue, t_stack *stack, char *token,
		char *next_token)
{
	static t_command	*last_command = NULL;

	if (queue == NULL || stack == NULL || token == NULL)
		return ;
	if (is_redirect(token))
	{
		add_redirect(last_command, token, next_token);
	}
	else if (is_operator(token))
	{
		add_operator(queue, stack, token);
		last_command = NULL;
	}
	else if (last_command == NULL)
	{
		last_command = create_command(token);
		command_enqueue_cmd(queue, last_command);
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

void	purge_stack(t_command_queue *queue, t_stack *stack)
{
	if (stack == NULL || queue == NULL)
		return ;
	while (stack_front(stack) != NULL)
	{
		command_enqueue_op(queue, stack_pop(stack));
	}
}

t_command_queue	*convert_postfix(t_token_list *tokens)
{
	t_command_queue		*queue;
	t_stack				*stack;
	char				*token;
	uint16_t			i;

	queue = create_command_queue();
	stack = stack_ctor(10);
	if (queue == NULL || queue->array == NULL
		|| stack == NULL || tokens == NULL)
	{
		if (queue != NULL)
			destroy_command_queue(&queue);
		if (stack != NULL)
			stack_dtor(&stack);
		return (NULL);
	}
	i = 0;
	while (i < token_list_size(tokens))
	{
		token = get_token(tokens, i);
		add_token(queue, stack, token, get_token(tokens, i + 1));
		++i;
	}
	purge_stack(queue, stack);
	stack_dtor(&stack);
	return (queue);
}
