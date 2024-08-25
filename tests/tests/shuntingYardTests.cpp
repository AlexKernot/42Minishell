/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shuntingYardTests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:41:56 by akernot           #+#    #+#             */
/*   Updated: 2024/08/25 19:30:53 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shuntingYardTests.hpp"
#include "test.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <unistd.h>

extern "C" {
	#include "shunting_yard.h"
	#include "ft_transmit.h"
}

static void transmitVec(int logFD, const std::string testName,
	const std::vector<std::string>& res,
	const std::vector<std::string>& cmp)
{
	std::stringstream strstr;
	strstr << "\e[0;31m" << testName << ": Comparison failed:\e[0m\nResult:  ";
	if (res.empty() == true)
		strstr << "(NULL) ";
	for (const std::string& str : res)
	{
		strstr << str << " ";
	}
	strstr << "\nCompare: ";
	if (cmp.empty() == true)
		strstr << "(NULL) ";
	for (const std::string& str : cmp)
	{
		strstr << str << " ";
	}
	strstr << "\n";
	transmit(logFD, strstr.str().c_str());
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
	{
		if (node->contents.contents.operator_word == NULL)
			result.push_back("(NULL)");
		else
			result.push_back(node->contents.contents.operator_word);
	}
	else
	{
		if (node->contents.contents.command == NULL)
			result.push_back("(NULL)");
		else
			result.push_back(node->contents.contents.command->command);
	}
}

static void expectTree(int logFD, std::string testName, t_syntax_tree *tree, t_stack *c_stack, t_stack *o_stack,
	const int c_size, const int o_size, const std::vector<std::string>& cmp)
{
	std::stringstream strstr;
	if (c_stack == NULL || o_stack == NULL) {
		strstr << "c_stack or o_stack is null\n";
		transmit(logFD, std::string(testName + ": " + strstr.str().c_str()).c_str());
		exit(1);
	}
	if (cmp.empty() == true) {
		if (tree == NULL)
			return ;
	}
	if (tree == NULL)
		strstr << "tree is (NULL)";
	if (c_stack->data->size != c_size)
		strstr << " c_stack expected " << c_size << " got " << c_stack->data->size;
	if (o_stack->data->size != o_size)
		strstr << " o_stack expected " << o_size << " got " << o_stack->data->size;
	std::vector<std::string> resVal;
	postfix(tree, resVal);
	if (cmp != resVal)
	{
		strstr << " comparison failed:\nExpected: ";
		if (cmp.empty() == true)
			strstr << "(NULL)";
		for (const std::string &a : cmp) {
			strstr << a << " ";
		}
		strstr << "\nReceived: ";
		if (resVal.empty() == true)
			strstr << "(NULL)";
		for (const std::string& a : resVal) {
			strstr << a << " ";
		}
		strstr << "\n";
	}
	if (strstr.str().size() != 0)
	{
		transmit(logFD, std::string(testName + ": " + strstr.str().c_str()).c_str());
		exit(1);
	}
}

void shuntingYardCreateSyntaxTree::test(int logFD) const
{
	t_syntax_tree *tree;
	create_syntax_tree(&tree, NULL, NULL, NULL);
	create_syntax_tree(&tree, createStr("c"),
		create_command(createStr("a")), create_command(createStr("b")));
	fassert(logFD, tree != NULL);
	fassert(logFD, tree->contents.type == op);
	fassert(logFD, std::string(tree->contents.contents.operator_word) == "c");
	fassert(logFD, tree->left != NULL);
	fassert(logFD, tree->right != NULL);
	fassert(logFD, tree->left->left == NULL);
	fassert(logFD, tree->left->right == NULL);
}

void shuntingYardPurgeOneCMD::test(int logFD) const
{
	t_syntax_tree *tree;
	create_syntax_tree(&tree, createStr("c"),
		create_command(createStr("a")), create_command(createStr("b")));
	t_stack *c_stack = stack_ctor(10);
	t_stack *o_stack = stack_ctor(10);
	t_syntax_tree *oldHead = tree;

	stack_push(c_stack, create_command(createStr("Test")));
	fassert(logFD, c_stack->data->size == 1);
	purge_stack(&tree, c_stack, o_stack);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, tree != oldHead);
	fassert(logFD, tree->contents.type == op);
	fassert(logFD, std::string(tree->contents.contents.operator_word) == ".");
	fassert(logFD, tree->left == oldHead);
	fassert(logFD, tree->right != NULL);
	fassert(logFD, tree->right->left == NULL);
	fassert(logFD, tree->right->right == NULL);
	fassert(logFD, tree->right->contents.type == command);
	fassert(logFD, std::string(tree->right->contents.contents.command->command) == "Test");
	
	delete_syntax_tree(&tree);
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	check_mem(logFD);
}

void shuntingYardPurgeOneCMDEmpty::test(int logFD) const
{
	t_syntax_tree *tree = NULL;
	t_stack *c_stack = stack_ctor(10);
	t_stack *o_stack = stack_ctor(10);

	stack_push(c_stack, create_command(createStr("Test")));
	fassert(logFD, c_stack->data->size == 1);
	fassert(logFD, o_stack->data->size == 0);
	purge_stack(&tree, c_stack, o_stack);
	fassert(logFD, tree != NULL);
	fassert(logFD, tree->right == NULL);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, tree->contents.type == op);
	fassert(logFD, std::string(tree->contents.contents.operator_word) == ".");
	fassert(logFD, tree->left->left == NULL);
	fassert(logFD, tree->left->right == NULL);
	fassert(logFD, tree->left->contents.type == command);
	fassert(logFD, std::string(tree->left->contents.contents.command->command) == "Test");
	
	delete_syntax_tree(&tree);
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	check_mem(logFD);
}

void shuntingYardPurgeStack::test(int logFD) const
{
	purge_stack(NULL, NULL, NULL);
	t_stack *c_stack = stack_ctor(10);
	fassert(logFD, c_stack != NULL);
	t_stack *o_stack = stack_ctor(10);
	fassert(logFD, o_stack != NULL);
	t_syntax_tree *tree = NULL;
	purge_stack(&tree, c_stack, o_stack);
	fassert(logFD, tree == NULL);

	stack_push(c_stack, create_command(createStr("d")));
	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("b")));
	fassert(logFD, c_stack->data->size == 3);
	
	stack_push(o_stack, createStr("e"));
	stack_push(o_stack, createStr("c"));
	fassert(logFD, o_stack->data->size == 2);

	purge_stack(NULL, c_stack, o_stack);
	fassert(logFD, c_stack->data->size == 3);
	fassert(logFD, o_stack->data->size == 2);

	purge_stack(&tree, c_stack, NULL);
	fassert(logFD, tree == NULL);
	fassert(logFD, c_stack->data->size == 3);
	
	purge_stack(&tree, NULL, o_stack);
	fassert(logFD, tree == NULL);
	fassert(logFD, o_stack->data->size == 2);

	purge_stack(&tree, c_stack, o_stack);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	std::vector<std::string> result;
	std::vector<std::string> cmp = {"a", "b", "c", "d", "e"};
	postfix(tree, result);
	if (result != cmp)
	{
		transmitVec(logFD, getTestName(), result, cmp);
		exit(1);
	}

	stack_push(c_stack, create_command(createStr("g")));
	fassert(logFD, c_stack->data->size == 1);
	stack_push(o_stack, createStr("h"));
	fassert(logFD, o_stack->data->size == 1);
	purge_stack(&tree, c_stack, o_stack);
	result.clear();
	cmp = {"a", "b", "c", "d", "e", "g", "h"};
	postfix(tree, result);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	if (result != cmp)
	{
		transmitVec(logFD, getTestName(), result, cmp);
		exit(1);
	}
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	delete_syntax_tree(&tree);
	check_mem(logFD);
}

void shuntingYardAddBrackets::test(int logFD) const
{
	t_syntax_tree *tree = NULL;
	t_stack *c_stack = stack_ctor(10);
	fassert(logFD, c_stack != NULL);
	t_stack *o_stack = stack_ctor(10);
	fassert(logFD, o_stack != NULL);
	char *bracket = createStr("(");
	fassert(logFD, bracket != NULL);
	
	add_brackets(NULL, NULL, NULL, NULL);
	add_brackets(NULL, c_stack, o_stack, bracket);
	add_brackets(&tree, NULL, o_stack, bracket);
	fassert(logFD, o_stack->data->size == 0);
	fassert(logFD, tree == NULL);
	add_brackets(&tree, c_stack, NULL, bracket);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, tree == NULL);
	add_brackets(&tree, c_stack, o_stack, NULL);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	fassert(logFD, tree == NULL);

	stack_push(c_stack, create_command(createStr("y")));
	stack_push(c_stack, create_command(createStr("d")));
	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("b")));
	fassert(logFD, c_stack->data->size == 4);

	stack_push(o_stack, createStr("z"));
	fassert(logFD, o_stack->data->size == 1);
	add_brackets(&tree, c_stack, o_stack, createStr("("));
	fassert(logFD, o_stack->data->size == 2);
	stack_push(o_stack, createStr("e"));
	stack_push(o_stack, createStr("c"));
	fassert(logFD, o_stack->data->size == 4);

	add_brackets(&tree, c_stack, o_stack, createStr(")"));
	fassert(logFD, o_stack->data->size == 1);
	std::vector<std::string> resVec;
	std::vector<std::string> cmp = {"a", "b", "c", "d", "e"};
	postfix(tree, resVec);
	if (resVec != cmp)
	{
		transmitVec(logFD, getTestName(), resVec, cmp);
		exit(1);
	}
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	delete_syntax_tree(&tree);
	check_mem(logFD);
}

void shuntingYardAddOperator::test(int logFD) const
{
	t_stack *o_stack = stack_ctor(10);
	fassert(logFD, o_stack != NULL);
	t_stack *c_stack = stack_ctor(10);
	fassert(logFD, c_stack != NULL);
	t_syntax_tree *tree = NULL;
	char *token = createStr("|");

	// Ensure NULL doesn't modify any values or allocate memory
	add_operator(NULL, NULL, NULL, NULL);
	add_operator(NULL, c_stack, o_stack, token);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	add_operator(&tree, NULL, o_stack, token);
	fassert(logFD, tree == NULL);
	fassert(logFD, o_stack->data->size == 0);
	add_operator(&tree, c_stack, o_stack, NULL);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	fassert(logFD, tree == NULL);

	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("a")));
	stack_push(c_stack, create_command(createStr("a")));
	
	// If an operator is pushed, check if it is pushed to the o_stack
	add_operator(&tree, c_stack, o_stack, token);
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 6, 1, {});
	
	// If another operator is pushed, check if the last operator is
	// added to the tree.
	add_operator(&tree, c_stack, o_stack, createStr("||"));
	fassert(logFD, tree != NULL);
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 4, 1, {"a", "a", "|"});
	
	// Ensure operator with lower precedence pushes operator with
	// higher precedence onto tree
	add_operator(&tree, c_stack, o_stack, createStr("a"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 3, 1, {"a", "a", "|", "a", "||"});

	// Higher precedence operators don't push lower precence operators
	// onto the tree.
	add_operator(&tree, c_stack, o_stack, createStr("||"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 3, 2, {"a", "a", "|", "a", "||"});

	// Ensure brackets don't push anything onto the tree
	add_operator(&tree, c_stack, o_stack, createStr("("));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 3, 3, {"a", "a", "|", "a", "||"});

	// Ensure bracket doesn't get pushed onto tree
	add_operator(&tree, c_stack, o_stack, createStr("|"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 3, 4, {"a", "a", "|", "a", "||"});

	// Ensure closed brackets push operators
	add_operator(&tree, c_stack, o_stack, createStr(")"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 2, 2, {"a", "a", "|", "a", "||", "a", "|"});
	
	stack_dtor(&c_stack);
	stack_dtor(&o_stack);
	delete_syntax_tree(&tree);
	check_mem(logFD);
}

void shuntingYardAddToken::test(int logFD) const
{
	t_syntax_tree *tree = NULL;
	t_stack *c_stack = stack_ctor(10);
	t_stack *o_stack = stack_ctor(10);
	fassert(logFD, c_stack != NULL);
	fassert(logFD, o_stack != NULL);

	char *token = createStr("command1");

	// Ensure NULL doesn't modify data or crash
	add_token(NULL, NULL, NULL, NULL);
	add_token(NULL, c_stack, o_stack, token);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	add_token(&tree, NULL, o_stack, token);
	fassert(logFD, tree == NULL);
	fassert(logFD, o_stack->data->size == 0);
	add_token(&tree, c_stack, NULL, token);
	fassert(logFD, tree == NULL);
	fassert(logFD, c_stack->data->size == 0);

	// Ensure phrase that isn't operator is added to c_stack
	add_token(&tree, c_stack, o_stack, token);
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 1, 0, {});
	fassert(logFD, ((t_command *)stack_front(c_stack))->args->size == 1);

	// Ensure phrase that isn't operator is added to cmd args
	add_token(&tree, c_stack, o_stack, createStr("Arg"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 1, 0, {});
	fassert(logFD, ((t_command *)stack_front(c_stack))->args->size == 2);

	// Add operator to o_stack
	add_token(&tree, c_stack, o_stack, createStr("|"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 1, 1, {});

	// Add another command
	add_token(&tree, c_stack, o_stack, createStr("command2"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 2, 1, {});

	// Make sure tree is constructed properly
	add_token(&tree, c_stack, o_stack, createStr("|"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 0, 1,
		{"command1", "command2", "|"});

	// Test brackets are handled correctly
	add_token(&tree, c_stack, o_stack, createStr("("));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 0, 2,
		{"command1", "command2", "|"});

	// Add command within brackets
	add_token(&tree, c_stack, o_stack, createStr("command4"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 1, 2,
		{"command1", "command2", "|"});

	add_token(&tree, c_stack, o_stack, createStr("&&"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 1, 3,
		{"command1", "command2", "|"});

	add_token(&tree, c_stack, o_stack, createStr("command3"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 2, 3,
		{"command1", "command2", "|"});
	
	// Close brackets
	add_token(&tree, c_stack, o_stack, createStr(")"));
	expectTree(logFD,  getTestName(), tree, c_stack, o_stack, 1, 1,
		{"command1", "command2", "|", "command3", "&&"});

	delete_syntax_tree(&tree);
	fassert(logFD, tree == NULL);
	stack_dtor(&c_stack);
	fassert(logFD, c_stack == NULL);
	stack_dtor(&o_stack);
	fassert(logFD, o_stack == NULL);
	check_mem(logFD);
}

void shuntingYardIsOperator::test(int logFD) const
{
	int result = is_operator(string);
	fassert(logFD, result == expected);
}

void shuntingYardIsRedirect::test(int logFD) const
{
	int result = is_redirect(string);
	fassert(logFD, result == expected);
}

void shuntingYardGetPrecedence::test(int logFD) const
{
	int result = get_precedence(string);
	fassert(logFD, result == expected);
}

void shuntingYardConvertPostfix::test(int logFD) const
{
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
		transmitVec(logFD, getTestName(), resVector, expected);
		exit(1);
	}
	delete_syntax_tree(&result);
	free(list);
	check_mem(logFD);
}


shuntingYardTestList::shuntingYardTestList()
	: fileTests("shuntingYardTest", {
		// Seperate into subtests!!!!
		new shuntingYardIsOperator("", false),
		new shuntingYardIsOperator(NULL, false),
		new shuntingYardIsOperator("a", false),
		new shuntingYardIsOperator("|", true),
		new shuntingYardIsOperator("&", false),
		new shuntingYardIsOperator("||", true),
		new shuntingYardIsOperator("&&", true),
		new shuntingYardIsOperator("?", false),
		new shuntingYardIsOperator("&&?", false),
		new shuntingYardIsOperator("|&", false),
		new shuntingYardIsOperator("&|", false),
		
		new shuntingYardIsRedirect(NULL, false),
		new shuntingYardIsRedirect("", false),
		new shuntingYardIsRedirect("<", true),
		new shuntingYardIsRedirect(">", true),
		new shuntingYardIsRedirect("<<", true),
		new shuntingYardIsRedirect(">>", true),
		new shuntingYardIsRedirect("<<aa", false),
		new shuntingYardIsRedirect("Aaa", false),
		new shuntingYardIsRedirect("><", false),
		new shuntingYardIsRedirect("<>", false),
		new shuntingYardIsRedirect("./", false),
		new shuntingYardIsRedirect("002", false),
		new shuntingYardIsRedirect("<ab", false),
		new shuntingYardIsRedirect(">\n", false),
		
		new shuntingYardGetPrecedence(NULL, 0),
		new shuntingYardGetPrecedence("", 0),
		new shuntingYardGetPrecedence("&&", 1),
		new shuntingYardGetPrecedence("||", 1),
		new shuntingYardGetPrecedence("|", 2),
		new shuntingYardGetPrecedence("(", 10),
		new shuntingYardGetPrecedence(")", 10),
		new shuntingYardGetPrecedence("A", 0),
		new shuntingYardGetPrecedence("<", 0),
		new shuntingYardGetPrecedence(">", 0),
		new shuntingYardGetPrecedence("|a", 0),
		new shuntingYardGetPrecedence("a|a", 0),
		new shuntingYardGetPrecedence("|&", 0),
		
		new shuntingYardCreateSyntaxTree(),
		new shuntingYardPurgeStack(),
		new shuntingYardPurgeOneCMD(),
		new shuntingYardPurgeOneCMDEmpty(),
		new shuntingYardAddBrackets(),
		new shuntingYardAddOperator(),
		new shuntingYardAddToken(),
		
		new shuntingYardConvertPostfix({}, {}),
		new shuntingYardConvertPostfix({"Command", "Arg1", "<<", "Redir"}, {"Command", "."}),
		new shuntingYardConvertPostfix({"Command1", "|", "Command2"}, {"Command1", "Command2", "|"}),
		new shuntingYardConvertPostfix({"Command1", "Arg1", "<<", "Redir", "|", "Command2", "Arg1", ">>", "Redir"}, {"Command1", "Command2", "|"}),
		new shuntingYardConvertPostfix({"Command1", "|", "Command2", "|", "Command3"}, {"Command1", "Command2", "|", "Command3", "|"})
	})
	{	}
