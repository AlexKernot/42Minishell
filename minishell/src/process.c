/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 23:17:38 by akernot           #+#    #+#             */
/*   Updated: 2024/08/12 14:56:59 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "syntax_tree.h"
#include "shunting_yard.h"
#include "evaluator.h"
#include "run.h"

static void print_tree(t_syntax_tree *tree, int level)
{
	if (tree == NULL)
		return ;
	for (int i = 0; i < level; ++i)
		write(STDOUT_FILENO, "-", 1);
	if (tree->contents.type == op)
		write(STDOUT_FILENO, tree->contents.contents.operator_word, ft_strlen(tree->contents.contents.operator_word));
	else
		write(STDOUT_FILENO, tree->contents.contents.command->command, ft_strlen(tree->contents.contents.command->command));
	write(STDOUT_FILENO, "\n", 1);
	print_tree(tree->right, level + 1);
	print_tree(tree->left, level + 1);
}

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

/**
 * @author Alex Kernot
 * @brief Run new command in a new process using fork so a signal like CTRL+C can
 * kill a running command without disrupting the terminal.
 * @param input the full command to parse and run.
*/
int	run(char *input)
{
	static int	last_return = 0;
	t_token_list	*tokens;
	t_syntax_tree	*tree;

	if (input == NULL || input[0] == '\0' || input[0] == '\n')
		return (0);
	tokens = tokenize(input);
	tree = convert_postfix(tokens);
	free(tokens->array);
	free(tokens);
	//print_tree(tree, 0);
	if (is_one_command(tree) == 1)
		last_return = run_without_subshell
			(tree->left->contents.contents.command);
	else
		last_return = evaluate_commands(tree,
			STDIN_FILENO, STDOUT_FILENO);
	delete_syntax_tree(&tree);
	return (last_return);
}
