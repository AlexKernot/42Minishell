/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenListTests.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:09:48 by akernot           #+#    #+#             */
/*   Updated: 2024/08/15 18:34:00 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include "unistd.h"

#include <iostream>

extern "C" {
	#include "token_list.h"
}

#include "tokenListTests.hpp"

#include "fileTests.hpp"

void tokenListBasicCreateAndDelete::test(int logFD) const
{
	t_token_list *list = create_token_list();
	fassert(logFD, list != NULL);
	fassert(logFD, list->array != NULL);
	fassert(logFD, list->size == 0);
	fassert(logFD, list->capacity == 10);
	delete_token_list(&list);
	fassert(logFD, list == NULL);
	check_mem(logFD);
}

void tokenListResizeTest::test(int logFD) const
{
	char *word1 = createStr("Word1");
	char *word2 = createStr("Word2");
	char *word3 = createStr("Word3");
	
	t_token_list *list = create_token_list();
	push_token(list, word1);
	push_token(list, word2);
	push_token(list, word3);
	char **arr = list->array;
	
	resize(list, 15);
	fassert(logFD, arr != list->array);
	fassert(logFD, list->capacity == 15);
	fassert(logFD, list->size == 3);
	fassert(logFD, list->array[0] == word1);
	fassert(logFD, list->array[1] == word2);
	fassert(logFD, list->array[2] == word3);
	
	char **arr2 = list->array;
	resize(list, 1);
	fassert(logFD, arr2 != list->array);
	fassert(logFD, list->capacity == 1);
	fassert(logFD, list->size == 1);
	fassert(logFD, list->array[0] == word1);

	char **arr3 = list->array;
	malloc_return_null();
	resize(list, 2);
	fassert(logFD, arr3 == list->array);
	fassert(logFD, list->capacity == 1);
	fassert(logFD, list->size == 1);
	malloc_revert();
	
	delete_token_list(&list);
	check_mem(logFD);
}

void tokenListPushTokenTest::test(int logFD) const
{
	char *word1 = createStr("Hello");
	char *word2 = createStr("Again");
	t_token_list *list = create_token_list();
	push_token(list, word1);
	push_token(list, word2);
	fassert(logFD, list->size == 2);
	fassert(logFD, list->array[0] == word1);
	fassert(logFD, list->array[1] == word2);

	for (int i = 0; i < 10; ++i)
	{
		push_token(list, word1);
	}
	fassert(logFD, list->capacity == 20);

	delete_token_list(&list);
	check_mem(logFD);
}

void tokenListGetTokenTest::test(int logFD) const
{
	char *word1 = createStr("Word1");
	char *word2 = createStr("Word2");
	char *word3 = createStr("Word3");
	t_token_list *list = create_token_list();
	push_token(list, word1);
	push_token(list, word2);
	push_token(list, word3);
	const char *res1 = get_token(list, 0);
	fassert(logFD, res1 == word1);

	const char *res2 = get_token(list, 1);
	fassert(logFD, res2 == word2);

	const char *res3 = get_token(list, 2);
	fassert(logFD, res3 == word3);

	const char *res4 = get_token(list, -1);
	fassert(logFD, res4 == NULL);

	const char *res5 = get_token(NULL, 0);
	fassert(logFD, res5 == NULL);

	const char *res6 = get_token(list, 100);
	fassert(logFD, res6 == NULL);
	
	const char *res7 = get_token(list, 3);
	fassert(logFD, res7 == NULL);
	
	delete_token_list(&list);
	t_token_list *list2 = new t_token_list;
	const char *res8 = get_token(list2, 0);
	fassert(logFD, res8 == NULL);
	delete list2;
	
	check_mem(logFD);
}

void tokenListSizeTest::test(int logFD) const
{
	char *word1 = createStr("one");
	char *word2 = createStr("two");
	char *word3 = createStr("three");

	t_token_list *list = create_token_list();

	fassert(logFD, token_list_size(NULL) == 0);
	t_token_list *invalid = new t_token_list;
	fassert(logFD, token_list_size(invalid) == 0);
	delete invalid;
	fassert(logFD, token_list_size(list) == 0);
	push_token(list, word1);
	fassert(logFD, token_list_size(list) == 1);
	push_token(list, word2);
	fassert(logFD, token_list_size(list) == 2);
	push_token(list, word3);
	fassert(logFD, token_list_size(list) == 3);
	resize(list, 1);
	fassert(logFD, token_list_size(list) == 1);
}

tokenListTestList::tokenListTestList()
	: fileTests("tokenListTests", {
		new tokenListBasicCreateAndDelete(),
		new tokenListResizeTest(),
		new tokenListPushTokenTest(),
		new tokenListGetTokenTest(),
		new tokenListSizeTest()
	})
	{	}
	