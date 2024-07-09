/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenListTests.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:09:48 by akernot           #+#    #+#             */
/*   Updated: 2024/07/09 17:09:27 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include "unistd.h"

#include <iostream>
#include <cassert>

extern "C" {
	#include "token_list.h"
}

bool basicCreateAndDelete()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		t_token_list *list = create_token_list();
		delete_token_list(&list);
		assert(list == NULL);
		check_mem();
		exit(0);
	}
	return returnWait(pid);
}

bool resizeTest()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		const char *word1 = "Word1";
		const char *word2 = "Word2";
		const char *word3 = "Word3";
		
		t_token_list *list = create_token_list();
		push_token(list, word1);
		push_token(list, word2);
		push_token(list, word3);
		const t_token *arr = list->array;
		
		resize(list, 15);
		assert(arr != list->array);
		assert(list->capacity == 15);
		assert(list->size == 3);
		assert(list->array[0].content == word1);
		assert(list->array[1].content == word2);
		assert(list->array[2].content == word3);
		
		const t_token *arr2 = list->array;
		resize(list, 1);
		assert(arr2 != list->array);
		assert(list->capacity == 1);
		assert(list->size == 1);
		assert(list->array[0].content == word1);

		const t_token *arr3 = list->array;
		malloc_return_null();
		resize(list, 2);
		assert(arr3 == list->array);
		assert(list->capacity == 1);
		assert(list->size == 1);
		malloc_revert();
		
		delete_token_list(&list);
		check_mem();
		exit(0);
	}
	return returnWait(pid);
}

bool pushTokenTest()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		const char *word1 = "Hello";
		const char *word2 = "Again";
		t_token_list *list = create_token_list();
		push_token(list, word1);
		push_token(list, word2);
		assert(list->size == 2);
		assert(list->array[0].content == word1);
		assert(list->array[1].content == word2);

		for (int i = 0; i < 10; ++i)
		{
			push_token(list, word1);
		}
		assert(list->capacity == 20);

		delete_token_list(&list);
		check_mem();
		exit(0);
	}
	return returnWait(pid);
}

bool getTokenTest()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		const char *word1 = "Word1";
		const char *word2 = "Word2";
		const char *word3 = "Word3";
		t_token_list *list = create_token_list();
		push_token(list, word1);
		push_token(list, word2);
		push_token(list, word3);
		const char *res1 = get_token(list, 0);
		assert(res1 == word1);

		const char *res2 = get_token(list, 1);
		assert(res2 == word2);

		const char *res3 = get_token(list, 2);
		assert(res3 == word3);

		const char *res4 = get_token(list, -1);
		assert(res4 == NULL);

		const char *res5 = get_token(NULL, 0);
		assert(res5 == NULL);

		const char *res6 = get_token(list, 100);
		assert(res6 == NULL);
		
		const char *res7 = get_token(list, 3);
		assert(res7 == NULL);
		
		delete_token_list(&list);
		t_token_list *list2 = new t_token_list;
		const char *res8 = get_token(list2, 0);
		assert(res8 == NULL);
		delete list2;
		
		check_mem();
		exit(0);
	}
	return returnWait(pid);
}

bool tokenListSizeTest()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		const char *word1 = "one";
		const char *word2 = "two";
		const char *word3 = "three";

		t_token_list *list = create_token_list();

		assert(token_list_size(NULL) == 0);
		t_token_list *invalid = new t_token_list;
		assert(token_list_size(invalid) == 0);
		delete invalid;
		assert(token_list_size(list) == 0);
		push_token(list, word1);
		assert(token_list_size(list) == 1);
		push_token(list, word2);
		assert(token_list_size(list) == 2);
		push_token(list, word3);
		assert(token_list_size(list) == 3);
		resize(list, 1);
		assert(token_list_size(list) == 1);
		exit(0);
	}
	return returnWait(pid);
}

std::pair<int, int> tokenListTests()
{
	int testsFailed = 0;
	int testsPassed = 0;

	basicCreateAndDelete() == false ? testsFailed++ : testsPassed++;
	resizeTest() == false ? testsFailed++ : testsPassed++;
	pushTokenTest() == false ? testsFailed++ : testsPassed++;
	getTokenTest() == false ? testsFailed++ : testsPassed++;
	tokenListSizeTest() == false ? testsFailed++ : testsPassed++;
	return {testsPassed, testsFailed};
}