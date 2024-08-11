/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxTreeTests.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:18:27 by akernot           #+#    #+#             */
/*   Updated: 2024/08/11 16:53:22 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntaxTreeTests.hpp"
#include "test.hpp"

#include <vector>
#include <string>
#include <source_location>

#include <unistd.h>

extern "C" {
	#include "syntax_tree.h"
}

void syntaxTreeCreateAndDestroy::test(int logFD) const
{
	check_mem(logFD);
	
	t_command *result = create_command(NULL);
	fassert(logFD, result != NULL);
	fassert(logFD, result->command == NULL);
	fassert(logFD, result->args != NULL);
	fassert(logFD, result->redirects != NULL);
	fassert(logFD, result->redir_types != NULL);
	fassert(logFD, result->args->size == 0);
	fassert(logFD, result->redirects->size == 0);
	fassert(logFD, result->redir_types->size == 0);
	delete_command(result);
	free(result);
	check_mem(logFD);

	result = create_command(createStr("Test"));
	fassert(logFD, result != NULL);
	fassert(logFD, std::string(result->command) == "Test");
	fassert(logFD, result->args != NULL);
	fassert(logFD, result->args->array != NULL);
	fassert(logFD, result->args->size == 1);
	fassert(logFD, result->redirects != NULL);
	fassert(logFD, result->redirects->array != NULL);
	fassert(logFD, result->redirects->size == 0);
	fassert(logFD, result->redir_types != NULL);
	fassert(logFD, result->redir_types->array != NULL);
	fassert(logFD, result->redir_types->size == 0);
	fassert(logFD, std::string(result->args->array[0].content) == "Test");
	fassert(logFD, result->command != result->args->array[0].content);
	delete_command(result);
	check_mem(logFD);

	result = create_command(createStr("Test2"));
	push_token(result->args, createStr("1"));
	push_token(result->args, createStr("2"));
	push_token(result->args, createStr("3"));
	fassert(logFD, result->args->size == 4);
	push_token(result->redir_types, createStr("1"));
	push_token(result->redir_types, createStr("2"));
	push_token(result->redir_types, createStr("3"));
	fassert(logFD, result->redir_types->size == 3);
	push_token(result->redirects, createStr("1"));
	push_token(result->redirects, createStr("2"));
	push_token(result->redirects, createStr("3"));
	fassert(logFD, result->redirects->size == 3);
	delete_command(result);
	check_mem(logFD);
}

void syntaxTreeAddRedirect::test(int logFD) const
{
	t_command *command = create_command(createStr("test"));
	fassert(logFD, command != NULL);
	char *string = createStr("File.txt");
	add_redirect(NULL, NULL, NULL, NULL);
	add_redirect(NULL, string, string, NULL);
	add_redirect(command, NULL, string, NULL);
	fassert(logFD, command->redir_types->size == 0);
	fassert(logFD, command->redirects->size == 0);
	add_redirect(command, string, NULL, NULL);
	fassert(logFD, command->redir_types->size == 0);
	fassert(logFD, command->redirects->size == 0);
	free(string);
	add_redirect(command, createStr("<"), createStr("file"), NULL);
	fassert(logFD, command->redir_types->size == 1);
	fassert(logFD, command->redirects->size == 1);
	fassert(logFD, std::string(command->redirects->array[0].content) == "file");
	fassert(logFD, std::string(command->redir_types->array[0].content) == "1");
	add_redirect(command, createStr("<<"), createStr("file2"), NULL);
	fassert(logFD, command->redir_types->size == 2);
	fassert(logFD, command->redirects->size == 2);
	fassert(logFD, std::string(command->redir_types->array[1].content) == "3");
	fassert(logFD, std::string(command->redirects->array[1].content) == "file2");
	add_redirect(command, createStr(">"), createStr("file3"), NULL);
	fassert(logFD, command->redir_types->size == 3);
	fassert(logFD, command->redirects->size == 3);
	fassert(logFD, std::string(command->redir_types->array[2].content) == "2");
	fassert(logFD, std::string(command->redirects->array[2].content) == "file3");
	add_redirect(command, createStr(">>"), createStr("file4"), NULL);
	fassert(logFD, command->redir_types->size == 4);
	fassert(logFD, command->redirects->size == 4);
	fassert(logFD, std::string(command->redirects->array[3].content) == "file4");
	fassert(logFD, std::string(command->redir_types->array[3].content) == "4");
	
	delete_command(command);
	check_mem(logFD);
}

void syntaxTreeAddArg::test(int logFD) const
{
	add_arg(NULL, NULL);
	t_command *result = create_command(createStr("command"));
	fassert(logFD, result != NULL);
	add_arg(result, NULL);
	fassert(logFD, result->args->size == 1);
	fassert(logFD, std::string(result->args->array[0].content) == "command");
	add_arg(result, createStr("1"));
	fassert(logFD, result->args->size == 2);
	fassert(logFD, std::string(result->args->array[1].content) == "1");
	delete_command(result);
	check_mem(logFD);

	result = create_command(NULL);
	fassert(logFD, result != NULL);
	fassert(logFD, result->args->size == 0);
	add_arg(result, createStr("1"));
	fassert(logFD, result->args->size == 1);
	fassert(logFD, std::string(result->command) == "1");
	fassert(logFD, std::string(result->args->array[0].content) == "1");
	delete_command(result);
	free(result);
	check_mem(logFD);
}

void syntaxTreeDelete::test(int logFD) const
{
	delete_syntax_tree(NULL);
	t_syntax_tree *null = NULL;
	delete_syntax_tree(&null);
	check_mem(logFD);

	t_syntax_tree *tree = (t_syntax_tree *)malloc(sizeof(*tree));
	tree->left = NULL;
	tree->right = NULL;
	tree->contents.type = command;
	tree->contents.contents.command = create_command(createStr("a"));
	delete_syntax_tree(&tree);
	fassert(logFD, tree == NULL);
	check_mem(logFD);

	tree = (t_syntax_tree *)malloc(sizeof(*tree));
	fassert(logFD, tree != NULL);
	tree->left = (t_syntax_tree *)malloc(sizeof(*tree));
	fassert(logFD, tree->left != NULL);
	tree->left->contents.type = op;
	tree->left->contents.contents.operator_word = createStr("a");
	tree->right = (t_syntax_tree *)malloc(sizeof(*tree));
	fassert(logFD, tree->right != NULL);
	tree->right->contents.type = command;
	tree->right->contents.contents.command = create_command(createStr("a"));
	
	tree->right->left = NULL;
	tree->right->right = NULL;
	tree->left->right = NULL;
	tree->left->left = (t_syntax_tree *)malloc(sizeof(*tree));
	fassert(logFD, tree->left->left != NULL);
	tree->left->left->contents.type = command;
	tree->left->left->contents.contents.command = NULL;
	tree->left->left->left = NULL;
	tree->left->left->right = NULL;
	delete_syntax_tree(&tree);
	fassert(logFD, tree == NULL);
	check_mem(logFD);
}

void syntaxTreeAddOp::test(int logFD) const
{
	add_op_to_syntax_tree(NULL, NULL, NULL);
	t_syntax_tree *head = NULL;
	t_command *cmd = create_command(createStr("a"));
	char *oper = createStr("b");
	add_op_to_syntax_tree(&head, oper, NULL);
	fassert(logFD, head == NULL);
	add_op_to_syntax_tree(&head, NULL, cmd);
	fassert(logFD, head == NULL);
	add_op_to_syntax_tree(&head, oper, cmd);
	fassert(logFD, head != NULL);
	fassert(logFD, head->contents.type == op);
	fassert(logFD, std::string(head->contents.contents.operator_word) == "b");
	fassert(logFD, head->left == NULL);
	fassert(logFD, head->right != NULL);
	fassert(logFD, head->right->left == NULL);
	fassert(logFD, head->right->right == NULL);
	fassert(logFD, head->right->contents.type == command);
	fassert(logFD, std::string(head->right->contents.contents.command->command) == "a");
	t_syntax_tree *oldHead = head;
	cmd = create_command(createStr("c"));
	oper = createStr("d");
	add_op_to_syntax_tree(&head, oper, cmd);
	fassert(logFD, head != NULL);
	fassert(logFD, head != oldHead);
	fassert(logFD, head->left == oldHead);
	fassert(logFD, head->right != NULL);
	fassert(logFD, head->contents.type == op);
	fassert(logFD, std::string(head->contents.contents.operator_word) == "d");
	fassert(logFD, head->right->contents.type == command);
	fassert(logFD, std::string(head->right->contents.contents.command->command) == "c");

	delete_syntax_tree(&head);
	fassert(logFD, head == NULL);
	check_mem(logFD);
}

void syntaxTreeAddCmd::test(int logFD) const
{
	create_syntax_tree(NULL, NULL, NULL, NULL);
	t_syntax_tree *tree = NULL;
	char *oper = createStr("Op");
	t_command *command1 = create_command(createStr("command1"));
	t_command *command2 = create_command(createStr("command2"));
	create_syntax_tree(&tree, NULL, command1, command2);
	fassert(logFD, tree == NULL);
	create_syntax_tree(&tree, oper, NULL, command2);
	fassert(logFD, tree == NULL);
	create_syntax_tree(&tree, oper, command1, NULL);
	fassert(logFD, tree == NULL);
	
	create_syntax_tree(&tree, oper, command1, command2);
	fassert(logFD, tree != NULL);
	fassert(logFD, tree->contents.type == op);
	fassert(logFD, std::string(tree->contents.contents.operator_word) == "Op");
	fassert(logFD, tree->left != NULL);
	fassert(logFD, tree->left->left == NULL);
	fassert(logFD, tree->left->right == NULL);
	fassert(logFD, tree->left->contents.type == command);
	fassert(logFD, std::string(tree->left->contents.contents.command->command) == "command1");
	fassert(logFD, tree->right != NULL);
	fassert(logFD, tree->right->left == NULL);
	fassert(logFD, tree->right->right == NULL);
	fassert(logFD, tree->right->contents.type == command);
	fassert(logFD, std::string(tree->right->contents.contents.command->command) == "command2");

	delete_syntax_tree(&tree);
	check_mem(logFD);
}

syntaxTreeTestList::syntaxTreeTestList()
	: fileTests("syntaxTreeTests", {
		new syntaxTreeCreateAndDestroy(),
		new syntaxTreeAddRedirect(),
		new syntaxTreeAddArg(),
		new syntaxTreeDelete(),
		new syntaxTreeAddOp(),
		new syntaxTreeAddCmd()
	})
	{	}