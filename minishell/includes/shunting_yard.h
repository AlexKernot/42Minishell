/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:21:18 by akernot           #+#    #+#             */
/*   Updated: 2024/08/07 15:54:19 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHUNTING_YARD_H
# define SHUNTING_YARD_H

# include "syntax_tree.h"
# include "stack.h"

t_syntax_tree *convert_postfix(t_token_list *tokens);
int		is_redirect(const char *token);
void		add_token(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack, char *token);
void		add_operator(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack, char *token);
int		is_operator(const char *token);
int		get_precedence(const char *token);
int		is_brackets(const char *token);
void		add_brackets(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack, char *token);
void		purge_stack(t_syntax_tree **tree, t_stack *c_stack,
			t_stack *o_stack);

#endif
