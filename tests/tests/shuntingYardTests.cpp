/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shuntingYardTests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:41:56 by akernot           #+#    #+#             */
/*   Updated: 2024/08/06 12:44:34 by akernot          ###   ########.fr       */
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
		result.push_back(node->contents.contents.operator_word);
	else
		result.push_back(node->contents.contents.command->command);
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

void shuntingYardPurgeStack::test(int logFD) const
{
	purge_stack(NULL, NULL, NULL);
	t_stack *c_stack = stack_ctor(10);
	fassert(logFD, c_stack != NULL);
	t_stack *o_stack = stack_ctor(10);
	fassert(logFD, o_stack != NULL);

	stack_push(c_stack, createStr("a"));
	stack_push(c_stack, createStr("b"));
	stack_push(c_stack, createStr("d"));
	stack_push(c_stack, createStr("e"));
	fassert(logFD, c_stack->data->size == 4);
	
	stack_push(o_stack, createStr("c"));
	stack_push(o_stack, createStr("f"));
	fassert(logFD, o_stack->data->size == 2);

	purge_stack(NULL, c_stack, o_stack);
	fassert(logFD, c_stack->data->size == 4);
	fassert(logFD, o_stack->data->size == 2);

	t_syntax_tree *tree = NULL;
	purge_stack(&tree, c_stack, NULL);
	fassert(logFD, tree == NULL);
	fassert(logFD, c_stack->data->size == 4);
	
	purge_stack(&tree, NULL, o_stack);
	fassert(logFD, tree == NULL);
	fassert(logFD, o_stack->data->size == 2);

	purge_stack(&tree, c_stack, o_stack);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	std::vector<std::string> result;
	std::vector<std::string> cmp = {"a", "b", "c", "d", "e", "f"};
	postfix(tree, result);
	if (result != cmp)
	{
		transmitVec(logFD, getTestName(), result, cmp);
		abort();
	}

	stack_push(c_stack, createStr("g"));
	stack_push(c_stack, createStr("h"));
	fassert(logFD, c_stack->data->size == 2);
	stack_push(o_stack, createStr("i"));
	fassert(logFD, o_stack->data->size == 1);
	purge_stack(&tree, c_stack, o_stack);
	result.clear();
	cmp = {"a", "b", "c", "d", "e", "f", "g", "h", "i"};
	postfix(tree, result);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	if (result != cmp)
	{
		transmitVec(logFD, getTestName(), result, cmp);
		abort();
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
	add_brackets(tree, NULL, o_stack, bracket);
	fassert(logFD, o_stack->data->size == 0);
	fassert(logFD, tree == NULL);
	add_brackets(tree, c_stack, NULL, bracket);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, tree == NULL);
	add_brackets(tree, c_stack, o_stack, NULL);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	fassert(logFD, tree == NULL);

	stack_push(c_stack, createStr("a"));
	stack_push(c_stack, createStr("b"));
	stack_push(c_stack, createStr("d"));
	stack_push(c_stack, createStr("e"));
	fassert(logFD, c_stack->data->size == 4);

	stack_push(o_stack, createStr("z"));
	fassert(logFD, o_stack->data->size == 1);
	add_brackets(tree, c_stack, o_stack, createStr("("));
	fassert(logFD, o_stack->data->size == 2);
	stack_push(o_stack, createStr("("));
	stack_push(o_stack, createStr("c"));
	stack_push(o_stack, createStr("f"));
	fassert(logFD, o_stack->data->size == 5);

	add_brackets(tree, c_stack, o_stack, createStr(")"));
	fassert(logFD, o_stack->data->size == 1);
	fassert(logFD, c_stack->data->size == 0);
	std::vector<std::string> resVec;
	std::vector<std::string> cmp = {"a", "b", "c", "d", "e", "f"};
	postfix(tree, resVec);
	if (resVec != cmp)
	{
		transmitVec(logFD, getTestName(), resVec, cmp);
		abort();
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
	add_operator(tree, NULL, o_stack, token);
	fassert(logFD, tree == NULL);
	fassert(logFD, o_stack->data->size == 0);
	add_operator(tree, c_stack, o_stack, NULL);
	fassert(logFD, c_stack->data->size == 0);
	fassert(logFD, o_stack->data->size == 0);
	fassert(logFD, tree == NULL);
	
	// If an operator is pushed, check if it is pushed to the o_stack
	add_operator(tree, c_stack, o_stack, token);
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 1, {}));
	
	// If another operator is pushed, check if the last operator is
	// added to the tree.
	add_operator(tree, c_stack, o_stack, createStr("||"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 1, {"|"}));
	
	// Ensure operator with lower precedence pushes operator with
	// higher precedence onto tree
	add_operator(tree, c_stack, o_stack, createStr("a"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 1, {"|", "||"}));

	// Higher precedence operators don't push lower precence operators
	// onto the tree.
	add_operator(tree, c_stack, o_stack, createStr("||"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 2, {"|", "||"}));

	// Ensure brackets don't push anything onto the tree
	add_operator(tree, c_stack, o_stack, createStr("("));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 3, {"|", "||"}));

	// Ensure bracket doesn't get pushed onto tree
	add_operator(tree, c_stack, o_stack, createStr("|"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 4, {"|", "||"}));

	// Ensure closed brackets push operators
	add_operator(tree, c_stack, o_stack, createStr(")"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 2, {"|", "||", "|"}));
	
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
	add_token(tree, NULL, o_stack, token);
	fassert(logFD, tree == NULL);
	fassert(logFD, o_stack->data->size == 0);
	add_token(tree, c_stack, NULL, token);
	fassert(logFD, tree == NULL);
	fassert(logFD, c_stack->data->size == 0);

	// Ensure phrase that isn't operator is added to c_stack
	add_token(tree, c_stack, o_stack, token);
	fassert(logFD, expectTree(tree, c_stack, o_stack, 1, 0, {}));
	fassert(logFD, ((t_command *)stack_front(o_stack))->args->size == 1);

	// Ensure phrase that isn't operator is added to cmd args
	add_token(tree, c_stack, o_stack, createStr("Arg"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 1, 0, {}));
	fassert(logFD, ((t_command *)stack_front(o_stack))->args->size == 2);

	// Add operator to o_stack
	add_token(tree, c_stack, o_stack, createStr("|"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 1, 1, {}));

	// Add another command
	add_token(tree, c_stack, o_stack, createStr("command2"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 2, 1, {}));

	// Make sure tree is constructed properly
	add_token(tree, c_stack, o_stack, createStr("|"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 1,
		{"command1", "command2", "|"}));

	// Test brackets are handled correctly
	add_token(tree, c_stack, o_stack, createStr("("));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 2,
		{"command1", "command2", "|"}));

	// Add command within brackets
	add_token(tree, c_stack, o_stack, createStr("command3"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 1, 2,
		{"command1", "command2", "|"}));

	add_token(tree, c_stack, o_stack, createStr("&&"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 1, 3,
		{"command1", "command2", "|"}));

	add_token(tree, c_stack, o_stack, createStr("command4"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 2, 2,
		{"command1", "command2", "|"}));
	
	// Close brackets
	add_token(tree, c_stack, o_stack, createStr(")"));
	fassert(logFD, expectTree(tree, c_stack, o_stack, 0, 1,
		{"command1", "command2", "|", "command3", "command4", "&&"}));

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
	delete_token_list(&list);
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
		
		new shuntingYardPurgeStack(),
		new shuntingYardAddBrackets(),
		new shuntingYardAddOperator(),
		new shuntingYardAddToken(),
		
		new shuntingYardConvertPostfix({}, {}),
		new shuntingYardConvertPostfix({"Command", "Arg1", "<<", "Redir"}, {"Command"}),
		new shuntingYardConvertPostfix({"Command1", "|", "Command2"}, {"Command1", "Command2", "|"}),
		new shuntingYardConvertPostfix({"Command1", "|", "Command2", "&&", "Command3"}, {"Command1", "Command2", "|", "Command3", "&&"}),
		new shuntingYardConvertPostfix({"Command1", "|", "(", "Command2", "&&", "Command3", ")"}, {"Command1", "Command2", "Command3", "&&", "|"})
	})
	{	}
