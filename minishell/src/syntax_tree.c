/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:24:08 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:30:40 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax_tree.h"

#include <stddef.h>
#include <stdlib.h>

#include "libft.h"

void	delete_command(t_command *command)
{
	if (command == NULL)
		return ;
	delete_token_list(&command->args);
	delete_token_list(&command->redirects);
	delete_token_list(&command->redir_types);
	if (command->command != NULL)
		free(command->command);
	command->command = NULL;
}

void	delete_command_item(t_inner_tree_item *item, t_op_or_cmd type)
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
		delete_command(item->command);
		free(item->command);
		item->command = NULL;
	}
}

void	delete_syntax_tree(t_syntax_tree **tree)
{
	t_syntax_tree	*t;

	if (tree == NULL || *tree == NULL)
		return ;
	t = *tree;
	delete_command_item(&t->contents.contents, t->contents.type);
	delete_syntax_tree(&t->left);
	delete_syntax_tree(&t->right);
	free(*tree);
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
	cmd->command = command;
	if (cmd->command != NULL)
		push_token(cmd->args, ft_strdup(command));
	return (cmd);
}

void	create_syntax_tree(t_syntax_tree **head, char *oper,
			t_command *command1, t_command *command2)
{
	t_syntax_tree	*node;

	if (command1 == NULL || command2 == NULL
		|| oper == NULL || head == NULL)
		return ;
	node = (t_syntax_tree *)malloc(sizeof(*node));
	if (node == NULL)
		return ;
	*head = node;
	node->contents.type = op;
	node->contents.contents.operator_word = oper;
	node->right = (t_syntax_tree *)malloc(sizeof(*node));
	node->left = (t_syntax_tree *)malloc(sizeof(*node));
	if (node->left != NULL)
		add_command(node->right, command1);
	if (node->right != NULL)
		add_command(node->left, command2);
}
