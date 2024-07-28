/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:57:24 by akernot           #+#    #+#             */
/*   Updated: 2024/07/28 01:35:51 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_TREE_H
# define SYNTAX_TREE_H

# include "token_list.h"

typedef struct s_command
{
	char		*command;
	t_token_list	*args;
	t_token_list	*redirects;
	t_token_list	*redir_types;
	uint16_t	size;
	uint16_t	capacity;
}	t_command;

typedef union u_inner_tree_item
{
	t_command	*command;
	char		*operator_word;
}	t_inner_tree_item;

typedef enum e_op_or_cmd
{
	op,
	command
}	t_op_or_cmd;

typedef struct s_tree_item
{
	t_inner_tree_item	contents;
	t_op_or_cmd		type;
}	t_tree_item;

typedef struct s_syntax_tree
{
	t_tree_item		contents;
	struct s_syntax_tree	*left;
	struct s_syntax_tree	*right;
}	t_syntax_tree;

t_command	*create_command(char *command);
void		add_op_to_syntax_tree(t_syntax_tree **head, char *oper,
			t_command *command);
void		add_cmd_to_syntax_tree(t_syntax_tree **head,
			t_command *command1, t_command *command2);
void		delete_syntax_tree(t_syntax_tree **tree);
void		delete_command(t_command *cmd);
void		delete_command_item(t_inner_tree_item *item, t_op_or_cmd type);

#endif
