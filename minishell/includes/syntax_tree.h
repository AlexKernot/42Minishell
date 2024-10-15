/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:57:24 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:13:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_TREE_H
# define SYNTAX_TREE_H

# include "token_list.h"
# include "stack.h"

typedef struct s_command
{
	char				*command;
	t_token_list		*args;
	t_token_list		*redirects;
	t_token_list		*redir_types;
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

typedef enum e_redirect_type
{
	redir_in = '1',
	redir_out = '2',
	redir_delim = '3',
	redir_append = '4',
	redir_none = '5'
}	t_redirect_type;

typedef struct s_tree_item
{
	t_inner_tree_item	contents;
	t_op_or_cmd			type;
}	t_tree_item;

typedef struct s_syntax_tree
{
	struct s_syntax_tree	*left;
	struct s_syntax_tree	*right;
	t_tree_item				contents;
}	t_syntax_tree;

t_command	*create_command(char *command);
void		add_arg(t_command *command, char *arg);
void		add_command(t_syntax_tree *node, t_command *command1);
void		add_redirect(t_command *command, char *redir, char *file,
				uint16_t *i);
void		add_op_to_syntax_tree(t_syntax_tree **head, char *oper,
				t_command *command);
void		create_syntax_tree(t_syntax_tree **head, char *oper,
				t_command *command1,
				t_command *command2);
void		delete_syntax_tree(t_syntax_tree **tree);
void		delete_command(t_command *cmd);
void		delete_command_item(t_inner_tree_item *item, t_op_or_cmd type);
void		purge_stack(t_syntax_tree **tree, t_stack *c_stack,
				t_stack *o_stack);
int			initialize_stacks(t_stack **o_stack, t_stack **c_stack);

#endif
