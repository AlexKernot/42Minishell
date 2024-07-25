/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shuntingYardTests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 16:41:56 by akernot           #+#    #+#             */
/*   Updated: 2024/07/25 17:26:31 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <cassert>

#include <unistd.h>

#include "test.hpp"

extern "C" {
	#include "shunting_yard.h"
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
	purge_stack(NULL, NULL);
	t_stack *stack = stack_ctor(10);
	assert(stack != NULL);
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	stack_push(stack, (void *)createStr("Test"));
	stack_push(stack, (void *)createStr("Test"));
	stack_push(stack, (void *)createStr("Test"));
	stack_push(stack, (void *)createStr("Test"));
	stack_push(stack, (void *)createStr("Test"));
	stack_push(stack, (void *)createStr("Test"));
	assert(stack->data->size == 6);
	purge_stack(queue, NULL);
	assert(queue->size == 0);

	purge_stack(NULL, stack);
	assert(stack->data->size == 6);
	
	assert(queue->size == 0);
	purge_stack(queue, stack);
	assert(stack->data->size == 0);
	assert(queue->size == 6);
	purge_stack(queue, stack);
	assert(stack->data->size == 0);
	assert(queue->size == 6);

	stack_dtor(&stack);
	assert(stack == NULL);
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();
	exit(0);
}

static bool testAddBrackets()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	t_stack *stack = stack_ctor(10);
	assert(stack != NULL);
	char *bracket = createStr("(");
	assert(bracket != NULL);
	add_brackets(NULL, NULL, NULL);
	add_brackets(NULL, stack, bracket);
	add_brackets(queue, NULL, bracket);
	add_brackets(queue, stack, NULL);

	stack_push(stack, createStr("|"));
	stack_push(stack, createStr("&&"));
	stack_push(stack, createStr("||"));
	assert(stack->data->size == 3);
	add_brackets(queue, stack, bracket);
	assert(queue->size == 0);
	assert(stack->data->size == 4);
	stack_push(stack, createStr("|"));
	assert(stack->data->size == 5);
	add_brackets(queue, stack, createStr("("));
	assert(queue->size == 0);
	assert(stack->data->size == 6);
	stack_push(stack, createStr("&&"));
	assert(stack->data->size == 7);

	add_brackets(queue, stack, createStr(")"));
	assert(queue->size == 1);
	assert(stack->data->size == 5);

	add_brackets(queue, stack, createStr(")"));
	assert(stack->data->size == 3);
	assert(queue->size == 2);

	destroy_command_queue(&queue);
	stack_dtor(&stack);
	check_mem();
	exit(0);
}

static bool testAddRedirect()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_command_queue	*queue = create_command_queue();
	assert(queue != NULL);
	t_command *command = create_command(createStr("test"));
	assert(command != NULL);
	command_enqueue_cmd(queue, command);
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

	destroy_command_queue(&queue);
	check_mem();
	exit(0);
}

static bool testAddOperator()
{
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_stack *stack = stack_ctor(10);
	t_command_queue *queue = create_command_queue();
	assert(stack != NULL);
	assert(queue != NULL);
	char *token = createStr("|");

	add_operator(NULL, NULL, NULL);
	add_operator(NULL, stack, token);
	add_operator(queue, NULL, token);
	add_operator(queue, stack, NULL);
	assert(queue->size == 0);
	assert(stack->data->size == 0);
	
	add_operator(queue, stack, token);
	assert(queue->size == 0);
	assert(stack->data->size == 1);
	
	add_operator(queue, stack, createStr("||"));
	assert(queue->size == 1);
	assert(stack->data->size == 1);
	
	add_operator(queue, stack, createStr("a"));
	assert(queue->size == 2);
	assert(stack->data->size == 1);

	add_operator(queue, stack, createStr("||"));
	assert(queue->size == 2);
	assert(stack->data->size == 2);

	add_operator(queue, stack, createStr("|"));
	assert(queue->size == 2);
	assert(stack->data->size == 3);

	add_operator(queue, stack, createStr("("));
	assert(queue->size == 2);
	assert(stack->data->size == 4);

	add_operator(queue, stack, createStr("|"));
	assert(queue->size == 2);
	assert(stack->data->size == 5);

	add_operator(queue, stack, createStr("||"));
	assert(queue->size == 3);
	assert(stack->data->size == 5);

	add_operator(queue, stack, createStr(")"));
	assert(queue->size == 4);
	assert(stack->data->size == 3);
	
	stack_dtor(&stack);
	destroy_command_queue(&queue);
	check_mem();
	exit(0);
}

static bool testAddToken()
{
	pid_t pid = fork();
	if (pid != 0) {
		return returnWait(pid);
	}
	t_command_queue *queue = create_command_queue();
	t_stack *stack = stack_ctor(10);
	assert(queue != NULL);
	assert(stack != NULL);

	char *token = createStr("command");

	add_token(NULL, NULL, NULL, NULL);
	add_token(NULL, stack, token, token);
	add_token(queue, NULL, token, token);
	add_token(queue, stack, NULL, token);

	add_token(queue, stack, token, NULL);
	assert(stack->data->size == 0);
	assert(queue->size == 1);
	assert(queue->array[0].type == command);
	
	add_token(queue, stack, createStr("args"), NULL);
	assert(queue->size == 1);
	assert(stack->data->size == 0);
	assert(queue->array[0].contents.command->args->size == 1);

	add_token(queue, stack, createStr("|"), NULL);
	assert(queue->size == 1);
	assert(stack->data->size == 1);

	add_token(queue, stack, createStr("command"), NULL);
	assert(queue->size == 2);
	assert(stack->data->size == 1);

	add_token(queue, stack, createStr("<<"), createStr("redir"));
	assert(queue->size == 2);
	assert(stack->data->size == 1);
	assert(std::string(queue->array[1].contents.command->redirects->array[0].content) == "redir");
	assert(std::string(queue->array[1].contents.command->redir_types->array[0].content) == "3");

	add_token(queue, stack, createStr("|"), NULL);
	assert(queue->size == 3);
	assert(stack->data->size == 1);
	
	add_token(queue, stack, createStr("("), NULL);
	assert(queue->size == 3);
	assert(stack->data->size == 2);
	
	add_token(queue, stack, createStr("command"), NULL);
	assert(queue->size == 4);
	assert(stack->data->size == 2);
	
	add_token(queue, stack, createStr("&&"), NULL);
	assert(queue->size == 4);
	assert(stack->data->size == 3);
	
	add_token(queue, stack, createStr("command"), NULL);
	assert(queue->size == 5);
	assert(stack->data->size == 3);
	
	add_token(queue, stack, createStr(")"), NULL);
	assert(queue->size == 6);
	assert(stack->data->size == 1);
	
	destroy_command_queue(&queue);
	stack_dtor(&stack);
	assert(queue == NULL);
	assert(stack == NULL);
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
	t_command_queue *result = convert_postfix(list);
	std::vector<std::string> resVector;
	while (queue_is_empty(result) == 0)
	{
		t_queue_item *queue_item = queue_front(result);
		if (queue_item->type == op)
		{
			resVector.push_back(queue_item->contents.operator_word);
		} else
		{
			resVector.push_back(queue_item->contents.command->command);
		}
		command_dequeue(result);
	}
	destroy_command_queue(&result);
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

	write(STDOUT_FILENO, "add_redirect() tests:\n", 22);
	testAddRedirect() == false ? testsFailed++ : testsPassed++;

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