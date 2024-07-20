/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_queue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:57:32 by akernot           #+#    #+#             */
/*   Updated: 2024/07/20 22:06:08 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_queue.h"

#include <stdlib.h>
#include "libft.h"
#include "token_list.h"

t_command_queue	*create_command_queue(void)
{
	t_command_queue	*queue;

	queue = (t_command_queue *)malloc(sizeof(*queue));
	if (queue == NULL)
		return (NULL);
	queue->array = (t_queue_item *)malloc(sizeof(*(queue->array)) * 10);
	if (queue->array == NULL)
	{
		free(queue);
		return (NULL);
	}
	queue->size = 0;
	queue->front = 0;
	queue->capacity = 10;
	return (queue);
}

t_command	*create_command(char *command)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(*cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = create_token_list();
	if (cmd->args == NULL)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirects = (t_redirect *)malloc(sizeof(*cmd->redirects) * 10);
	if (cmd->redirects == NULL)
	{
		delete_token_list(&cmd->args);
		free(cmd);
		return (NULL);
	}
	cmd->size = 0;
	cmd->capacity = 10;
	cmd->command = command;
	return (cmd);
}

static void	destroy_command(t_command *command)
{
	t_redirect	*redirects;
	uint16_t	i;

	if (command == NULL)
		return ;
	redirects = command->redirects;
	delete_token_list(&command->args);
	i = 0;
	while (i < command->size && redirects != NULL)
	{
		if (redirects[i].file_name != NULL)
			free(redirects[i].file_name);
		++i;
	}
	if (redirects != NULL)
		free(redirects);
	command->redirects = NULL;
	if (command->command != NULL)
		free(command->command);
	command->command = NULL;
	command->size = 0;
	command->capacity = 0;
}

static void	destroy_command_item(t_inner_queue_item *item, t_op_or_cmd type)
{
	if (item == NULL)
		return ;
	if (type == op && item->operator_word != NULL)
	{
		free(item->operator_word);
		item->operator_word = NULL;
	}
	else if (type == command && item->command != NULL)
	{
		destroy_command(item->command);
		free(item->command);
		item->command = NULL;
	}
}

void	destroy_command_queue(t_command_queue **queue)
{
	uint16_t		i;
	t_queue_item	*item;

	i = 0;
	if (queue == NULL || *queue == NULL)
		return ;
	while (i < (*queue)->size)
	{
		item = &(*queue)->array[i];
		destroy_command_item(&item->contents, item->type);
		++i;
	}
	free(*queue);
	*queue = NULL;
}

static uint16_t	get_min(uint16_t a, uint16_t b)
{
	if (a < b)
		return (a);
	return (b);
}

void		queue_resize(t_command_queue *queue, uint16_t size)
{
	uint16_t	i;
	t_queue_item	*new_array;

	if (queue == NULL || size == 0)
		return ;
	i = 0;
	new_array = malloc(sizeof(*new_array) * size);
	if (new_array == NULL)
		return ;
	while (i < queue->size)
	{
		if (i >= size)
		{
			destroy_command_item(&queue->array[i].contents,
				queue->array[i].type);
			++i;
			continue;
		}
		new_array[i].contents = queue->array[i].contents;
		new_array[i].type = queue->array[i].type;
		++i;
	}
	queue->array = new_array;
	queue->capacity = size;
	queue->size = get_min(queue->size, size);
}

void	command_enqueue_op(t_command_queue *queue, char *string)
{
	if (queue == NULL || queue->array == NULL || string == NULL)
		return ;
	if (queue->size >= queue->capacity)
		queue_resize(queue, queue->size * 2);
	queue->array[queue->size].type = op;
	queue->array[queue->size].contents.operator_word = string;
	queue->size++;
}

void	command_enqueue_cmd(t_command_queue *queue, t_command *cmd)
{
	if (queue == NULL || queue->array == NULL || cmd == NULL)
		return ;
	if (queue->size >= queue->capacity)
		queue_resize(queue, queue->size * 2);
	queue->array[queue->size].type = command;
	queue->array[queue->size].contents.command = cmd;
	queue->size++;
}

void	command_dequeue(t_command_queue *queue)
{
	if (queue == NULL || queue->array == NULL)
		return ;
	if (queue->front >= queue->size || queue->size == 0)
		return ;
	queue->front++;
}

t_queue_item	*queue_front(const t_command_queue *queue)
{
	if (queue == NULL || queue->array == NULL)
		return (NULL);
	if (queue->front >= queue->size)
		return (NULL);
	return (&queue->array[queue->front]);
}

uint16_t	queue_size(const t_command_queue *queue)
{
	if (queue == NULL)
		return (0);
	if (queue->front >= queue->size)
		return (0);
	return (queue->size - queue->front);
}

int	queue_is_empty(const t_command_queue *queue)
{
	if (queue == NULL)
		return (1);
	if (queue->front >= queue->size)
		return (1);
	return (0);
}
