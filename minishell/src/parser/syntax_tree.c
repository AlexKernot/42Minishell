/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:24:08 by akernot           #+#    #+#             */
/*   Updated: 2024/08/08 09:48:10 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax_tree.h"

#include <stddef.h>
#include <stdlib.h>

#include "libft.h"

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

void		delete_syntax_tree(t_syntax_tree **tree)
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

void	add_arg(t_command *command, char *arg)
{
	if (command == NULL || arg == NULL)
		return ;
	if (command->command == NULL)
		command->command = ft_strdup(arg);
	push_token(command->args, arg);
}

void		add_op_to_syntax_tree(t_syntax_tree **head, char *oper,
			t_command *cmd)
{
	t_syntax_tree *node;

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

static void add_command(t_syntax_tree *node, t_command *command1)
{
	if (node == NULL || command1 == NULL)
		return ;
	node->left = NULL;
	node->right = NULL;
	node->contents.type = command;
	node->contents.contents.command = command1;
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
		add_command(node->left, command1);
	if (node->right != NULL)
		add_command(node->right, command2);
}
