/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:24:08 by akernot           #+#    #+#             */
/*   Updated: 2024/07/28 01:35:49 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax_tree.h"

#include <stddef.h>
#include <stdlib.h>

static void	destroy_command(t_command *command)
{
	if (command == NULL)
		return ;
	delete_token_list(&command->args);
	delete_token_list(&command->redirects);
	delete_token_list(&command->redir_types);
	if (command->command != NULL)
		free(command->command);
	command->command = NULL;
	command->size = 0;
	command->capacity = 0;
}

static void	destroy_command_item(t_inner_tree_item *item, t_op_or_cmd type)
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

void		delete_syntax_tree(t_syntax_tree **tree)
{
	t_syntax_tree	*t;

	if (tree == NULL || *tree == NULL)
		return ;
	t = *tree;
	destroy_command_item(&t->contents.contents, t->contents.type);
	delete_syntax_tree(&t->left);
	delete_syntax_tree(&t->right);
	*tree = NULL;
}

t_command	*create_command(char *command)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(*cmd));
	if (cmd == NULL)
		return (NULL);
	cmd->args = create_token_list();
	cmd->redirects = create_token_list();
	cmd->redir_types = create_token_list();
	if (cmd->args == NULL || cmd->redirects == NULL
		|| cmd->redir_types == NULL)
	{
		if (cmd->args != NULL)
			delete_token_list(&cmd->args);
		if (cmd->redirects != NULL)
			delete_token_list(&cmd->redirects);
		if (cmd->redir_types != NULL)
			delete_token_list(&cmd->redir_types);
		free(cmd);
		return (NULL);
	}
	cmd->size = 0;
	cmd->capacity = 10;
	cmd->command = command;
	return (cmd);
}

void		add_op_to_syntax_tree(t_syntax_tree **head, char *oper,
			t_command *cmd)
{
	t_syntax_tree *node;

	if (oper == NULL || cmd == NULL || head == NULL)
		return ;
	node = (t_syntax_tree *)malloc(sizeof(*node));
	if (*head == NULL)
		*head = node;
	node->right = NULL;
	node->contents.type = op;
	node->contents.contents.operator_word = oper;
	node->left = (t_syntax_tree *)malloc(sizeof(t_syntax_tree));
	if (node->left == NULL)
		return ;
	node->left->left = NULL;
	node->left->right = NULL;
	node->left->contents.type = command;
	node->left->contents.contents.command = cmd;
	if (node != *head)
		node->right = *head;
}

void	add_cmd_to_syntax_tree(t_syntax_tree **head,
			t_command *command1, t_command *command2)
{
	t_syntax_tree	*node;

	if (command1 == NULL || command2 == NULL || head == NULL)
		return ;
	node = (t_syntax_tree *)malloc(sizeof(*node));
	if (*head == NULL)
		*head = node;
	node->right = (t_syntax_tree *)malloc(sizeof(*node));
	node->left = (t_syntax_tree *)malloc(sizeof(*node));
	if (node->left != NULL)
	{
		node->left->contents.contents.command = command1;
		node->left->contents.type = command;
		node->left->left = NULL;
		node->left->right = NULL;
	}
	if (node->right != NULL)
	{
		node->left->contents.contents.command = command2;
		node->right->contents.type = command;
		node->right->left = NULL;
		node->right->right = NULL;
	}
}
