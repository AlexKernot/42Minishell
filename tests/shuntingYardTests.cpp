/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shuntingYardTests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:41:56 by akernot           #+#    #+#             */
/*   Updated: 2024/07/31 13:45:27 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

#include <unistd.h>

#include "test.hpp"

extern "C" {
	#include "shunting_yard.h"
}

static void printVecs(std::vector<std::string> res, std::vector<std::string> cmp)
{
	std::cout << "Comparison failed:\nResult:  ";
	for (const std::string& str : res)
	{
		std::cout << str << " ";
	}
	std::cout << "\nCompare: ";
	for (const std::string& str : cmp)
	{
		std::cout << str << " ";
	}
	std::cout << std::endl;
}

static void postfix(t_syntax_tree *node, std::vector<std::string>& result)
{
	if (node == NULL)
		return ;
	if (node->left != NULL)
		postfix(node->left, result);
	if (node->right != NULL)
		postfix(node->right, result);
	
	if (node->contents.type == op)
		result.push_back(node->contents.contents.operator_word);
	else
		result.push_back(node->contents.contents.command->command);
}

static bool testIsOperator(const char *string, bool expected)
{
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	int result = is_operator(string);
	if (result == 1 && expected == true)
		exit(0);
	if (result == 0 && expected == false)
		exit(0);
	exit(1);
}

static bool testIsRedirect(const char *string, bool expected)
{
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	int result = is_redirect(string);
	if (result == 1 && expected == true)
		exit(0);
	if (result == 0 && expected == false)
		exit(0);
	exit(1);
}

static bool testPrecedence(const char *string, int expected)
{
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	int result = get_precedence(string);
	if (result == expected)
		exit(0);
	exit(1);
}

static bool testPurgeStack()
{
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	purge_stack(NULL, NULL, NULL);
	t_stack *c_stack = stack_ctor(10);
	assert(c_stack != NULL);
	t_stack *o_stack = stack_ctor(10);
	assert(o_stack != NULL);

	stack_push(c_stack, createStr("a"));
	stack_push(c_stack, createStr("b"));
	stack_push(c_stack, createStr("d"));
	stack_push(c_stack, createStr("e"));
	assert(c_stack->data->size == 4);
	
	stack_push(o_stack, createStr("c"));
	stack_push(o_stack, createStr("f"));
	assert(o_stack->data->size == 2);

	purge_stack(NULL, c_stack, o_stack);
	assert(c_stack->data->size == 4);
	assert(o_stack->data->size == 2);

	t_syntax_tree *tree = NULL;
	purge_stack(&tree, c_stack, NULL);
	assert(tree == NULL);
	assert(c_stack->data->size == 4);
	
	purge_stack(&tree, NULL, o_stack);
	assert(tree == NULL);
	assert(o_stack->data->size == 2);

	purge_stack(&tree, c_stack, o_stack);
	assert(c_stack->data->size == 0);
	assert(o_stack->data->size == 0);
	std::vector<std::string> result;
	std::vector<std::string> cmd = {"a", "b", "c", "d", "e", "f"};
	postfix(tree, result);
	if (result != cmd)
	{
		printVecs(result, cmd);
		abort();
	}

	stack_push(c_stack, createStr("g"));
	stack_push(c_stack, createStr("h"));
	assert(c_stack->data->size == 2);
	stack_push(o_stack, createStr("i"));
	assert(o_stack->data->size == 1);
	purge_stack(&tree, c_stack, o_stack);
	result.clear();
	cmd = {"a", "b", "c", "d", "e", "f", "g", "h", "i"};
	postfix(tree, result);
	assert(c_stack->data->size == 0);
	assert(o_stack->data->size == 0);
	if (result != cmd)
	{
		printVecs(result, cmd);
		abort();
	}
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	delete_syntax_tree(&tree);
	check_mem();
	exit(0);
}

static bool testAddBrackets()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_syntax_tree *tree = NULL;
	t_stack *c_stack = stack_ctor(10);
	assert(c_stack != NULL);
	t_stack *o_stack = stack_ctor(10);
	assert(o_stack != NULL);
	char *bracket = createStr("(");
	assert(bracket != NULL);
	
	add_brackets(NULL, NULL, NULL, NULL);
	add_brackets(NULL, c_stack, o_stack, bracket);
	add_brackets(tree, NULL, o_stack, bracket);
	assert(o_stack->data->size == 0);
	assert(tree == NULL);
	add_brackets(tree, c_stack, NULL, bracket);
	assert(c_stack->data->size == 0);
	assert(tree == NULL);
	add_brackets(tree, c_stack, o_stack, NULL);
	assert(c_stack->data->size == 0);
	assert(o_stack->data->size == 0);
	assert(tree == NULL);

	stack_push(c_stack, createStr("a"));
	stack_push(c_stack, createStr("b"));
	stack_push(c_stack, createStr("d"));
	stack_push(c_stack, createStr("e"));
	assert(c_stack->data->size == 4);

	stack_push(o_stack, createStr("z"));
	assert(o_stack->data->size == 1);
	add_brackets(tree, c_stack, o_stack, createStr("("));
	assert(o_stack->data->size == 2);
	stack_push(o_stack, createStr("("));
	stack_push(o_stack, createStr("c"));
	stack_push(o_stack, createStr("f"));
	assert(o_stack->data->size == 5);

	add_brackets(tree, c_stack, o_stack, createStr(")"));
	assert(o_stack->data->size == 1);
	assert(c_stack->data->size == 0);
	std::vector<std::string> resVec;
	std::vector<std::string> cmp = {"a", "b", "c", "d", "e", "f"};
	postfix(tree, resVec);
	if (resVec != cmp)
	{
		printVecs(resVec, cmp);
		abort();
	}
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	delete_syntax_tree(&tree);
	check_mem();
	exit(0);
}

static bool expectTree(t_syntax_tree *tree, t_stack *c_stack, t_stack *o_stack,
	const int c_size, const int o_size, const std::vector<std::string>& cmp)
{
	if (c_stack == NULL || o_stack == NULL)
		return false;
	if (cmp.empty() == true) {
		if (tree == NULL)
			return true;
		return false;
	}
	if (tree == NULL)
		return false;
	if (c_stack->data->size != c_size)
		return false;
	if (o_stack->data->size != o_size)
		return false;
	std::vector<std::string> resVal;
	postfix(tree, resVal);
	if (cmp != resVal)
		return false;
	return true;
}

static bool testAddOperator()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_stack *o_stack = stack_ctor(10);
	assert(o_stack != NULL);
	t_stack *c_stack = stack_ctor(10);
	assert(c_stack != NULL);
	t_syntax_tree *tree = NULL;
	char *token = createStr("|");

	// Ensure NULL doesn't modify any values or allocate memory
	add_operator(NULL, NULL, NULL, NULL);
	add_operator(NULL, c_stack, o_stack, token);
	assert(c_stack->data->size == 0);
	assert(o_stack->data->size == 0);
	add_operator(tree, NULL, o_stack, token);
	assert(tree == NULL);
	assert(o_stack->data->size == 0);
	add_operator(tree, c_stack, o_stack, NULL);
	assert(c_stack->data->size == 0);
	assert(o_stack->data->size == 0);
	assert(tree == NULL);
	
	// If an operator is pushed, check if it is pushed to the o_stack
	add_operator(tree, c_stack, o_stack, token);
	assert(expectTree(tree, c_stack, o_stack, 0, 1, {}));
	
	// If another operator is pushed, check if the last operator is
	// added to the tree.
	add_operator(tree, c_stack, o_stack, createStr("||"));
	assert(expectTree(tree, c_stack, o_stack, 0, 1, {"|"}));
	
	// Ensure operator with lower precedence pushes operator with
	// higher precedence onto tree
	add_operator(tree, c_stack, o_stack, createStr("a"));
	assert(expectTree(tree, c_stack, o_stack, 0, 1, {"|", "||"}));

	// Higher precedence operators don't push lower precence operators
	// onto the tree.
	add_operator(tree, c_stack, o_stack, createStr("||"));
	assert(expectTree(tree, c_stack, o_stack, 0, 2, {"|", "||"}));

	// Ensure brackets don't push anything onto the tree
	add_operator(tree, c_stack, o_stack, createStr("("));
	assert(expectTree(tree, c_stack, o_stack, 0, 3, {"|", "||"}));

	// Ensure bracket doesn't get pushed onto tree
	add_operator(tree, c_stack, o_stack, createStr("|"));
	assert(expectTree(tree, c_stack, o_stack, 0, 4, {"|", "||"}));

	// Ensure closed brackets push operators
	add_operator(tree, c_stack, o_stack, createStr(")"));
	assert(expectTree(tree, c_stack, o_stack, 0, 2, {"|", "||", "|"}));
	
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	delete_syntax_tree(&tree);
	check_mem();
	exit(0);
}

static bool testAddToken()
{
	pid_t pid = fork();
	if (pid != 0) {
		return returnWait(pid);
	}
	t_syntax_tree *tree = NULL;
	t_stack *c_stack = stack_ctor(10);
	t_stack *o_stack = stack_ctor(10);
	assert(c_stack != NULL);
	assert(o_stack != NULL);

	char *token = createStr("command1");

	// Ensure NULL doesn't modify data or crash
	add_token(NULL, NULL, NULL, NULL);
	add_token(NULL, c_stack, o_stack, token);
	assert(c_stack->data->size == 0);
	assert(o_stack->data->size == 0);
	add_token(tree, NULL, o_stack, token);
	assert(tree == NULL);
	assert(o_stack->data->size == 0);
	add_token(tree, c_stack, NULL, token);
	assert(tree == NULL);
	assert(c_stack->data->size == 0);

	// Ensure phrase that isn't operator is added to c_stack
	add_token(tree, c_stack, o_stack, token);
	assert(expectTree(tree, c_stack, o_stack, 1, 0, {}));
	assert(((t_command *)stack_front(o_stack))->args->size == 1);

	// Ensure phrase that isn't operator is added to cmd args
	add_token(tree, c_stack, o_stack, createStr("Arg"));
	assert(expectTree(tree, c_stack, o_stack, 1, 0, {}));
	assert(((t_command *)stack_front(o_stack))->args->size == 2);

	// Add operator to o_stack
	add_token(tree, c_stack, o_stack, createStr("|"));
	assert(expectTree(tree, c_stack, o_stack, 1, 1, {}));

	// Add another command
	add_token(tree, c_stack, o_stack, createStr("command2"));
	assert(expectTree(tree, c_stack, o_stack, 2, 1, {}));

	// Make sure tree is constructed properly
	add_token(tree, c_stack, o_stack, createStr("|"));
	assert(expectTree(tree, c_stack, o_stack, 0, 1,
		{"command1", "command2", "|"}));

	// Test brackets are handled correctly
	add_token(tree, c_stack, o_stack, createStr("("));
	assert(expectTree(tree, c_stack, o_stack, 0, 2,
		{"command1", "command2", "|"}));

	// Add command within brackets
	add_token(tree, c_stack, o_stack, createStr("command3"));
	assert(expectTree(tree, c_stack, o_stack, 1, 2,
		{"command1", "command2", "|"}));

	add_token(tree, c_stack, o_stack, createStr("&&"));
	assert(expectTree(tree, c_stack, o_stack, 1, 3,
		{"command1", "command2", "|"}));

	add_token(tree, c_stack, o_stack, createStr("command4"));
	assert(expectTree(tree, c_stack, o_stack, 2, 2,
		{"command1", "command2", "|"}));
	
	// Close brackets
	add_token(tree, c_stack, o_stack, createStr(")"));
	assert(expectTree(tree, c_stack, o_stack, 0, 1,
		{"command1", "command2", "|", "command3", "command4", "&&"}));

	delete_syntax_tree(&tree);
	assert(tree == NULL);
	stack_dtor(&c_stack);
	assert(c_stack == NULL);
	stack_dtor(&o_stack);
	assert(o_stack == NULL);
	check_mem();
	exit(0);
}

static bool testConvertPostfix(const std::vector<std::string>& in, 
		const std::vector<std::string>& expected)
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_token_list *list = create_token_list();
	if (list != NULL) {
		for (const std::string& a : in) {
			push_token(list, createStr(a.c_str()));
		}
	}
	t_syntax_tree *result = convert_postfix(list);
	std::vector<std::string> resVector;
	postfix(result, resVector);
	if (resVector != expected)
	{
		printVecs(resVector, expected);
		exit(1);
	}
	delete_syntax_tree(&result);
	delete_token_list(&list);
	check_mem();
	if (resVector == expected) {
		exit(0);
	}
	exit(1);
}

std::pair<int, int> shuntingYardTest()
{
	int testsPassed = 0;
	int testsFailed = 0;

	write(STDOUT_FILENO, "is_operator() tests:\n", 21);
	testIsOperator("", false) == false ? testsFailed++ : testsPassed++;
	testIsOperator(NULL, false) == false ? testsFailed++ : testsPassed++;
	testIsOperator("a", false) == false ? testsFailed++ : testsPassed++;
	testIsOperator("|", true) == false ? testsFailed++ : testsPassed++;
	testIsOperator("&", false) == false ? testsFailed++ : testsPassed++;
	testIsOperator("||", true) == false ? testsFailed++ : testsPassed++;
	testIsOperator("&&", true) == false ? testsFailed++ : testsPassed++;
	testIsOperator("?", false) == false ? testsFailed++ : testsPassed++;
	testIsOperator("&&?", false) == false ? testsFailed++ : testsPassed++;
	testIsOperator("|&", false) == false ? testsFailed++ : testsPassed++;
	testIsOperator("&|", false) == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "is_redirect() tests:\n", 21);
	testIsRedirect(NULL, false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("<", true) == false ? testsFailed++ : testsPassed++;
	testIsRedirect(">", true) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("<<", true) == false ? testsFailed++ : testsPassed++;
	testIsRedirect(">>", true) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("<<aa", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("Aaa", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("><", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("<>", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("./", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("002", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect("<ab", false) == false ? testsFailed++ : testsPassed++;
	testIsRedirect(">\n", false) == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "get_precedence() tests:\n", 24);
	testPrecedence(NULL, 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence("", 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence("&&", 1) == false ? testsFailed++ : testsPassed++;
	testPrecedence("||", 1) == false ? testsFailed++ : testsPassed++;
	testPrecedence("|", 2) == false ? testsFailed++ : testsPassed++;
	testPrecedence("(", 10) == false ? testsFailed++ : testsPassed++;
	testPrecedence(")", 10) == false ? testsFailed++ : testsPassed++;
	testPrecedence("A", 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence("<", 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence(">", 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence("|a", 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence("a|a", 0) == false ? testsFailed++ : testsPassed++;
	testPrecedence("|&", 0) == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "purge_stack() tests:\n", 21);
	testPurgeStack() == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "add_brackets() tests:\n", 22);
	testAddBrackets() == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "add_operator() tests:\n", 22);
	testAddOperator() == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "add_token() tests:\n", 19);
	testAddToken() == false ? testsFailed++ : testsPassed++;

	write(STDOUT_FILENO, "convert_postfix() tests:\n", 25);
	testConvertPostfix({}, {}) == false ? testsFailed++ : testsPassed++;
	malloc_return_null();
	testConvertPostfix({"Test", "|", "Test"}, {}) == false ? testsFailed++ : testsPassed++;
	malloc_revert();
	testConvertPostfix({"Command", "Arg1", "<<", "Redir"}, {"Command"}) == false ? testsFailed++ : testsPassed++;
	testConvertPostfix({"Command1", "|", "Command2"}, {"Command1", "Command2", "|"}) == false ? testsFailed++ : testsPassed++;
	testConvertPostfix({"Command1", "|", "Command2", "&&", "Command3"}, {"Command1", "Command2", "|", "Command3", "&&"}) == false ? testsFailed++ : testsPassed++;
	testConvertPostfix({"Command1", "|", "(", "Command2", "&&", "Command3", ")"}, {"Command1", "Command2", "Command3", "&&", "|"}) == false ? testsFailed++ : testsPassed++;
	return {testsPassed, testsFailed};
}