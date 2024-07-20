/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandQueueTests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:03:54 by akernot           #+#    #+#             */
/*   Updated: 2024/07/20 21:23:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <cassert>
#include <unistd.h>

extern "C" {
	#include "command_queue.h"
}

static char *createStr(const char *text)
{
	char *retVal = (char *)malloc(strlen(text) + 1);
	strcpy(retVal, text);
	retVal[strlen(text)] = '\0';
	return retVal;
}

static bool createCommandQueue()
{
	write(STDOUT_FILENO, "Create command queue:\n", 22);
	pid_t pid = fork();
	if (pid == 0)
	{
		t_command_queue	*queue = create_command_queue();
		assert(queue != NULL);
		assert(queue->size == 0);
		assert(queue->front == 0);
		assert(queue->capacity == 10);
		assert(queue->array != NULL);
		malloc_return_null();
		queue = create_command_queue();
		assert(queue == NULL);
		malloc_revert();
		exit(0);
	}
	return returnWait(pid);
}

static bool destroyCommandQueue()
{
	write(STDOUT_FILENO, "Destroy command queue:\n", 23);
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();

	queue = create_command_queue();
	assert(queue != NULL);
	queue->size = 1;
	queue->array->type = command;
	t_command *cmd = queue->array->contents.command;
	cmd = (t_command *)malloc(sizeof(*cmd));
	cmd->command = (char *)malloc(sizeof(char) * 4);
	cmd->args = create_token_list();
	cmd->redirects = (t_redirect *)malloc(sizeof(t_redirect));
	cmd->redirects[0].file_name = (char *)malloc(sizeof(char) * 4);
	cmd->size = 1;
	cmd->capacity = 1;
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();

	queue = create_command_queue();
	assert(queue != NULL);
	queue->size = 1;
	queue->array[0].type = command;
	cmd = create_command(createStr("Test"));
	queue->array[0].contents.command = cmd;
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();
	exit(0);
}

static bool queueEnqueue()
{
	write(STDOUT_FILENO, "Enqueue:\n", 9);
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	assert(queue->array != NULL);
	char *text = createStr("Test");
	t_command *cmd = create_command(text);
	assert(cmd != NULL);
	assert(queue->size == 0);
	
	command_enqueue_cmd(NULL, cmd);
	assert(queue->size == 0);
	char *test = createStr("Test");
	command_enqueue_op(NULL, test);
	assert(queue->size == 0);
	free(test);
	command_enqueue_cmd(queue, NULL);
	assert(queue->size == 0);
	command_enqueue_op(queue, NULL);
	assert(queue->size == 0);
	
	command_enqueue_cmd(queue, cmd);
	assert(queue->size == 1);
	assert(queue->front == 0);
	assert(queue->array[0].type == command);
	assert(queue->array[0].contents.command == cmd);

	char *oper = createStr("Test");
	command_enqueue_op(queue, oper);
	assert(queue->size == 2);
	assert(queue->front == 0);
	assert(queue->array[1].type == op);
	assert(queue->array[1].contents.operator_word == oper);
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();
	exit(0);
}

static bool queueDequeue()
{
	write(STDOUT_FILENO, "Dequeue:\n", 9);
	pid_t pid = fork();
	if (pid != 0)
	{
		return returnWait(pid);
	}
	command_dequeue(NULL);
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	assert(queue->array != NULL);
	char *text = (char *)malloc(5);
	strcpy(text, "Test");
	t_command *cmd = create_command(text);
	assert(cmd != NULL);
	command_enqueue_cmd(queue, cmd);
	
	command_dequeue(queue);
	command_dequeue(queue);
	assert(queue->size == queue->front);
	assert(queue->front == 1);
	
	destroy_command_queue(&queue);
	check_mem();
	exit(0);
}

static bool queueResize()
{
	write(STDOUT_FILENO, "Resize:\n", 8);
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	exit(0);
}

static bool queueFront()
{
	write(STDOUT_FILENO, "Front:\n", 7);
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	assert(queue_front(NULL) == NULL);
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	assert(queue_front(queue) == NULL);
	t_command *cmdOne = create_command(createStr("Test"));
	command_enqueue_cmd(queue, cmdOne);
	t_queue_item *front = queue_front(queue);
	assert(front->type == command);
	assert(front->contents.command == cmdOne);
	command_enqueue_op(queue, createStr("Type"));
	front = queue_front(queue);
	assert(front != NULL);
	assert(front->type == command);
	assert(front->contents.command == cmdOne);
	command_dequeue(queue);
	front = queue_front(queue);
	assert(front != NULL);
	assert(front->type == op);
	assert(std::string(front->contents.operator_word) == "Type");
	command_dequeue(queue);
	front = queue_front(queue);
	assert(front == NULL);
	
	destroy_command_queue(&queue);
	assert(queue == NULL);	
	check_mem();
	exit(0);
}

static bool queueSize()
{
	write(STDOUT_FILENO, "Size:\n", 6);
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	
	assert(queue_size(NULL) == 0);
	t_queue_item *array = queue->array;
	queue->array = NULL;
	assert(queue_size(queue) == 0);
	queue->array = array;
	assert(queue_size(queue) == 0);
	command_enqueue_cmd(queue, create_command(createStr("Test")));
	assert(queue_size(queue) == 1);
	command_enqueue_cmd(queue, create_command(createStr("Test")));
	assert(queue_size(queue) == 2);
	command_enqueue_cmd(queue, create_command(createStr("Test")));
	assert(queue_size(queue) == 3);
	command_dequeue(queue);
	assert(queue_size(queue) == 2);
	command_dequeue(queue);
	assert(queue_size(queue) == 1);
	command_dequeue(queue);
	assert(queue_size(queue) == 0);
	command_dequeue(queue);
	assert(queue_size(queue) == 0);
	command_enqueue_op(queue, createStr("test"));
	assert(queue_size(queue) == 1);
	
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();
	exit(0);
}

static bool queueIsEmpty()
{
	write(STDOUT_FILENO, "Empty:\n", 7);
	pid_t pid = fork();
	if (pid != 0)
		return returnWait(pid);
	t_command_queue *queue = create_command_queue();
	assert(queue != NULL);
	assert(queue_is_empty(NULL) == 1);
	assert(queue_is_empty(queue) == 1);
	command_enqueue_cmd(queue, create_command(createStr("Test")));
	assert(queue_is_empty(queue) == 0);
	command_dequeue(queue);
	assert(queue_is_empty(queue) == 1);
	destroy_command_queue(&queue);
	assert(queue == NULL);
	check_mem();
	exit(0);
}

std::pair<int, int> commandQueueTest()
{
	int	testsPassed = 0;
	int	testsFailed = 0;

	createCommandQueue() == false ? testsFailed++ : testsPassed++;
	destroyCommandQueue() == false ? testsFailed++ : testsPassed++;
	queueResize() == false ? testsFailed++ : testsPassed++;
	queueEnqueue() == false ? testsFailed++ : testsPassed++;
	queueDequeue() == false ? testsFailed++ : testsPassed++;
	queueFront() == false ? testsFailed++ : testsPassed++;
	queueSize() == false ? testsFailed++ : testsPassed++;
	queueIsEmpty() == false ? testsFailed++ : testsPassed++;

	return {testsPassed, testsFailed};
}