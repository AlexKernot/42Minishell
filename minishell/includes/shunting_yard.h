/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:21:18 by akernot           #+#    #+#             */
/*   Updated: 2024/07/25 16:53:22 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHUNTING_YARD_H
# define SHUNTING_YARD_H

# include "command_queue.h"
# include "stack.h"

t_command_queue *convert_postfix(t_token_list *tokens);
int		is_redirect(const char *token);
void		add_token(t_command_queue *queue, t_stack *stack, char *token,
			char *next_token);
void		add_operator(t_command_queue *queue, t_stack *stack,
			char *token);
int		is_operator(const char *token);
int		get_precedence(const char *token);
int		is_brackets(const char *token);
void		add_brackets(t_command_queue *queue, t_stack *stack,
			char *token);
void		add_redirect(t_command *command, char *redir, char *file);
void		purge_stack(t_command_queue *queue, t_stack *stack);

#endif
