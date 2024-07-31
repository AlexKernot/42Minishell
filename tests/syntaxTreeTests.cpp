/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxTreeTests.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:18:27 by akernot           #+#    #+#             */
/*   Updated: 2024/07/31 13:56:21 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <vector>
#include <string>
#include <cassert>

#include <unistd.h>

extern "C" {
	#include "syntax_tree.h"
}

static bool createAndDestroyCommandTest()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	malloc_return_null();
	t_command *result = create_command(NULL);
	malloc_revert();
	assert(result == NULL);
	
	result = create_command(NULL);
	assert(result != NULL);
	assert(result->command == NULL);
	assert(result->args != NULL);
	assert(result->redirects != NULL);
	assert(result->redir_types != NULL);
	assert(result->args->size == 0);
	assert(result->redirects->size == 0);
	assert(result->redir_types->size == 0);
	delete_command(result);
	check_mem();

	result = create_command(createStr("Test"));
	assert(result != NULL);
	assert(std::string(result->command) == "Test");
	assert(result->args != NULL);
	assert(result->args->array != NULL);
	assert(result->args->size == 1);
	assert(result->redirects != NULL);
	assert(result->redirects->array != NULL);
	assert(result->redirects->size == 0);
	assert(result->redir_types != NULL);
	assert(result->redir_types->array != NULL);
	assert(result->redir_types->size == 0);
	assert(std::string(result->args->array[0].content) == "Test");
	assert(result->command != result->args->array[0].content);
	delete_command(result);
	check_mem();

	result = create_command(createStr("Test2"));
	push_token(result->args, createStr("1"));
	push_token(result->args, createStr("2"));
	push_token(result->args, createStr("3"));
	assert(result->args->size == 4);
	push_token(result->redir_types, createStr("1"));
	push_token(result->redir_types, createStr("2"));
	push_token(result->redir_types, createStr("3"));
	assert(result->redir_types->size == 3);
	push_token(result->redirects, createStr("1"));
	push_token(result->redirects, createStr("2"));
	push_token(result->redirects, createStr("3"));
	assert(result->redirects->size == 3);
	delete_command(result);
	check_mem();
	
	exit(0);
}

static bool addRedirectTest()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_command *command = create_command(createStr("test"));
	assert(command != NULL);
	char *string = createStr("File.txt");
	add_redirect(NULL, NULL, NULL);
	add_redirect(NULL, string, string);
	add_redirect(command, NULL, string);
	assert(command->redir_types->size == 0);
	assert(command->redirects->size == 0);
	add_redirect(command, string, NULL);
	assert(command->redir_types->size == 0);
	assert(command->redirects->size == 0);
	free(string);
	add_redirect(command, createStr("<"), createStr("file"));
	assert(command->redir_types->size == 1);
	assert(command->redirects->size == 1);
	assert(std::string(command->redirects->array[0].content) == "file");
	assert(std::string(command->redir_types->array[0].content) == "1");
	add_redirect(command, createStr("<<"), createStr("file2"));
	assert(command->redir_types->size == 2);
	assert(command->redirects->size == 2);
	assert(std::string(command->redir_types->array[1].content) == "3");
	assert(std::string(command->redirects->array[1].content) == "file2");
	add_redirect(command, createStr(">"), createStr("file3"));
	assert(command->redir_types->size == 3);
	assert(command->redirects->size == 3);
	assert(std::string(command->redir_types->array[2].content) == "2");
	assert(std::string(command->redirects->array[2].content) == "file3");
	add_redirect(command, createStr(">>"), createStr("file4"));
	assert(command->redir_types->size == 4);
	assert(command->redirects->size == 4);
	assert(std::string(command->redirects->array[3].content) == "file4");
	assert(std::string(command->redir_types->array[3].content) == "4");
	
	delete_command(command);
	check_mem();
	exit(0);
}

static bool addArgTest()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	add_arg(NULL, NULL);
	t_command *result = create_command(createStr("command"));
	assert(result != NULL);
	add_arg(result, NULL);
	assert(result->args->size == 1);
	assert(std::string(result->args->array[0].content) == "command");
	add_arg(result, createStr("1"));
	assert(result->args->size == 2);
	assert(std::string(result->args->array[1].content) == "1");
	delete_command(result);
	check_mem();

	result = create_command(NULL);
	assert(result != NULL);
	assert(result->args->size == 0);
	add_arg(result, createStr("1"));
	assert(result->args->size == 1);
	assert(std::string(result->command) == "1");
	assert(std::string(result->args->array[0].content) == "1");
	delete_command(result);
	check_mem();
	exit(0);
}

static bool deleteSyntaxTree()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	delete_syntax_tree(NULL);
	t_syntax_tree *null = NULL;
	delete_syntax_tree(&null);

	t_syntax_tree *tree = (t_syntax_tree *)malloc(sizeof(*tree));
	tree->left = NULL;
	tree->right = NULL;
	tree->contents.type = command;
	tree->contents.contents.command = create_command(createStr("a"));
	delete_syntax_tree(&tree);
	assert(tree == NULL);
	check_mem();

	tree = (t_syntax_tree *)malloc(sizeof(*tree));
	assert(tree != NULL);
	tree->left = (t_syntax_tree *)malloc(sizeof(*tree));
	assert(tree->left != NULL);
	tree->left->contents.type = op;
	tree->left->contents.contents.operator_word = createStr("a");
	tree->right = (t_syntax_tree *)malloc(sizeof(*tree));
	assert(tree->right != NULL);
	tree->right->contents.type = command;
	tree->right->contents.contents.command = create_command(createStr("a"));
	
	tree->right->left = NULL;
	tree->right->right = NULL;
	tree->left->right = NULL;
	tree->left->left = (t_syntax_tree *)malloc(sizeof(*tree));
	assert(tree->left->left != NULL);
	tree->left->left->contents.type = command;
	tree->left->left->contents.contents.command = NULL;
	tree->left->left->left = NULL;
	tree->left->left->right = NULL;
	delete_syntax_tree(&tree);
	assert(tree == NULL);
	check_mem();
	exit(0);
}

static bool addOpToSyntaxTree()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	add_op_to_syntax_tree(NULL, NULL, NULL);
	t_syntax_tree *head = NULL;
	t_command *cmd = create_command(createStr("a"));
	char *oper = createStr("b");
	add_op_to_syntax_tree(&head, oper, NULL);
	assert(head == NULL);
	add_op_to_syntax_tree(&head, NULL, cmd);
	assert(head == NULL);
	add_op_to_syntax_tree(&head, oper, cmd);
	assert(head != NULL);
	assert(head->contents.type == op);
	assert(std::string(head->contents.contents.operator_word) == "b");
	assert(head->left == NULL);
	assert(head->right != NULL);
	assert(head->right->left == NULL);
	assert(head->right->right == NULL);
	assert(head->right->contents.type == command);
	assert(std::string(head->right->contents.contents.command->command) == "a");
	t_syntax_tree *oldHead = head;
	cmd = create_command(createStr("c"));
	oper = createStr("d");
	add_op_to_syntax_tree(&head, oper, cmd);
	assert(head != NULL);
	assert(head != oldHead);
	assert(head->left == oldHead);
	assert(head->right != NULL);
	assert(head->contents.type == op);
	assert(std::string(head->contents.contents.operator_word) == "d");
	assert(head->right->contents.type == command);
	assert(std::string(head->right->contents.contents.command->command) == "c");

	delete_syntax_tree(&head);
	assert(head == NULL);
	check_mem();
	exit(0);
}

static bool addCmdToSyntaxTree()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	create_syntax_tree(NULL, NULL, NULL, NULL);
	t_syntax_tree *tree = NULL;
	char *oper = createStr("Op");
	t_command *command1 = create_command(createStr("command1"));
	t_command *command2 = create_command(createStr("command2"));
	create_syntax_tree(&tree, NULL, command1, command2);
	assert(tree == NULL);
	create_syntax_tree(&tree, oper, NULL, command2);
	assert(tree == NULL);
	create_syntax_tree(&tree, oper, command1, NULL);
	assert(tree == NULL);
	
	create_syntax_tree(&tree, oper, command1, command2);
	assert(tree != NULL);
	assert(tree->contents.type == op);
	assert(std::string(tree->contents.contents.operator_word) == "Op");
	assert(tree->left != NULL);
	assert(tree->left->left == NULL);
	assert(tree->left->right == NULL);
	assert(tree->left->contents.type == command);
	assert(std::string(tree->left->contents.contents.command->command) == "command1");
	assert(tree->right != NULL);
	assert(tree->right->left == NULL);
	assert(tree->right->right == NULL);
	assert(tree->right->contents.type == command);
	assert(std::string(tree->right->contents.contents.command->command) == "command2");

	t_syntax_tree *oldHead = tree;
	create_syntax_tree(&tree, oper, command1, command2);
	assert(tree == oldHead);
	delete_syntax_tree(&tree);
	check_mem();
	exit(0);
}

std::pair<int, int> syntaxTreeTests()
{
	int testsPassed = 0;
	int testsFailed = 0;
	
	write(STDOUT_FILENO, "\nCreate and destroy command:\n", 29);
	createAndDestroyCommandTest() == false ? testsFailed++ : testsPassed++; 
	write(STDOUT_FILENO, "\nadd_redirect():\n", 17);
	addRedirectTest() == false ? testsFailed++ : testsPassed++;
	write(STDOUT_FILENO, "\nadd_arg():\n", 12);
	addArgTest() == false ? testsFailed++ : testsPassed++;
	write(STDOUT_FILENO, "\ndelete_syntax_tree():\n", 23);
	deleteSyntaxTree() == false ? testsFailed++ : testsPassed++;
	write(STDOUT_FILENO, "\nadd_op_to_syntax_tree():\n", 26);
	addOpToSyntaxTree() == false ? testsFailed++ : testsPassed++;
	write(STDOUT_FILENO, "\ncreate_syntax_tree():\n", 27);
	addCmdToSyntaxTree() == false ? testsFailed++ : testsPassed++;
	return {testsPassed, testsFailed};
}