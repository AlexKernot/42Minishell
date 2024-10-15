/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_syntax_tree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:29:43 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:30:44 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax_tree.h"

#include <stddef.h>
#include <stdlib.h>

#include "libft.h"

void	add_redirect(t_command *command, char *redir, char *file, uint16_t *i)
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
	free(redir);
	push_token(command->redir_types, redir_type);
	push_token(command->redirects, file);
	if (i != NULL)
		(*i)++;
}

void	add_arg(t_command *command, char *arg)
{
	if (command == NULL || arg == NULL)
		return ;
	if (command->command == NULL)
		command->command = ft_strdup(arg);
	push_token(command->args, arg);
}

void	add_command(t_syntax_tree *node, t_command *command1)
{
	if (node == NULL || command1 == NULL)
		return ;
	node->left = NULL;
	node->right = NULL;
	node->contents.type = command;
	node->contents.contents.command = command1;
}

void	add_op_to_syntax_tree(t_syntax_tree **head, char *oper,
			t_command *cmd)
{
	t_syntax_tree	*node;

	if (oper == NULL || cmd == NULL || head == NULL)
		return ;
	node = (t_syntax_tree *)malloc(sizeof(*node));
	if (node == NULL)
		return ;
	node->left = *head;
	*head = node;
	node->contents.type = op;
	node->contents.contents.operator_word = oper;
	node->right = (t_syntax_tree *)malloc(sizeof(t_syntax_tree));
	if (node->right == NULL)
		return ;
	node->right->left = NULL;
	node->right->right = NULL;
	node->right->contents.type = command;
	node->right->contents.contents.command = cmd;
	if (node != *head)
		node->right = *head;
}
