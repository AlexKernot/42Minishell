/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_queue.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:57:50 by akernot           #+#    #+#             */
/*   Updated: 2024/07/21 13:05:05 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include <stdint.h>

#include "token_list.h"

typedef enum e_redir_type
{
	redir_in,
	redir_out,
	redir_delim,
	redir_append
}	t_redir_type;

typedef struct s_redirect
{
	char		*file_name;
	t_redir_type	type;
}	t_redirect;

typedef struct s_command
{
	char		*command;
	t_token_list	*args;
	t_token_list	*redirects;
	t_token_list	*redir_types;
	uint16_t	size;
	uint16_t	capacity;
}	t_command;

typedef union u_inner_queue_item
{
	t_command	*command;
	char		*operator_word;
}	t_inner_queue_item;

typedef enum e_op_or_cmd
{
	op,
	command
}	t_op_or_cmd;

typedef struct s_queue_item
{
	t_inner_queue_item	contents;
	t_op_or_cmd		type;
}	t_queue_item;

typedef struct s_command_queue
{
	t_queue_item	*array;
	uint16_t	size;
	uint16_t	capacity;
	uint16_t	front;
}	t_command_queue;

t_command_queue	*create_command_queue(void);
t_command	*create_command(char *command);

void		queue_resize(t_command_queue *queue, uint16_t size);
void		destroy_command_queue(t_command_queue **queue);
void		command_enqueue_op(t_command_queue *queue, char *string);
void		command_enqueue_cmd(t_command_queue *queue, t_command *cmd);
void		command_dequeue(t_command_queue *queue);
t_queue_item	*queue_front(const t_command_queue *queue);
uint16_t	queue_size(const t_command_queue *queue);
int		queue_is_empty(const t_command_queue *queue);

#endif
