/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 23:17:38 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 22:16:37 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "libft.h"
#include "tokenizer.h"
#include "syntax_tree.h"
#include "shunting_yard.h"
#include "evaluator.h"
#include "run.h"
#include "environment_variables.h"
#include "expand.h"

static int is_one_command(t_syntax_tree *tree)
{
	if (tree == NULL)
		return (0);
	if (tree->contents.type != op)
		return (0);
	if (tree->contents.contents.operator_word[0] != '.'
		|| tree->contents.contents.operator_word[1] != '\0')
		return (0);
	if (tree->right != NULL)
		return (0);
	if (tree->left->contents.type == command)
		return (1);
	return (0);
}

static int	run_tree(t_syntax_tree *tree, int last_return)
{
	int ret;

	if (is_one_command(tree) == 1)
	{
		ret = run_without_subshell
			(tree->left->contents.contents.command, last_return);
		return ret;
	}
	else
	{
		return (evaluate_commands(tree,
			STDIN_FILENO, STDOUT_FILENO, last_return));
	}
}

/**
 * @author Alex Kernot
 * @brief Run new command in a new process using fork so a signal like CTRL+C
 * can kill a running command without disrupting the terminal.
 * @param input the full command to parse and run.
*/
int	run(char *input)
{
	static int	last_return = 0;
	char		*expanded;
	t_token_list	*tokens;
	t_syntax_tree	*tree;

	if (input == NULL || input[0] == '\0' || input[0] == '\n')
		return (0);
	expanded = expand(input, last_return);
	if (ft_strlen(expanded) == 0)
	{
		last_return = 0;
		return (0);
	}
	if (expanded == NULL)
		expanded = input;
	tokens = tokenize(expanded);
	free(expanded);
	tree = convert_postfix(tokens);
	free(tokens->array);
	free(tokens);
	last_return = run_tree(tree, last_return);
	delete_syntax_tree(&tree);
	return (last_return);
}
