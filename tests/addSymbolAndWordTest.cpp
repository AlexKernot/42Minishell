/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addSymbolAndWordTest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:46:25 by akernot           #+#    #+#             */
/*   Updated: 2024/07/10 15:56:06 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <string>
#include <vector>
#include <iostream>

#include <unistd.h>

extern "C" {
	#include "tokenizer.h"	
}

static void print_list(t_token_list *list)
{
	std::cout << "{";
	for (uint16_t i = 0; i < list->size; ++i)
	{
		std::cout << "\'" << list->array[i].content << "\' ";
	}
	std::cout << "}" << std::endl;
}

static bool operator==(const t_token_list *a, const std::vector<std::string>& b)
{
	if (a->size != b.size())
		return false;
	for (std::size_t i = 0; i < b.size(); ++i)
	{
		if (get_token(a, i) != b.at(i))
			return false;
	}
	return true;
}

static bool operator!=(const t_token_list *a, const std::vector<std::string>& b)
{
	if (a == b)
		return false;
	return true;
}

static bool addSymbolTest(const char *string, const uint16_t start,
		const uint16_t end, std::vector<std::string> cmp)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		t_token_list *result = create_token_list();
		add_symbols(result, string, start, end);
		if (result != cmp) {
			print_list(result);
			exit(1);
		}
		exit(0);
	}
	return returnWait(pid);
}

static bool addWordTest(const char *string, const uint16_t start,
		const uint16_t end, std::vector<std::string> cmp)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		t_token_list *result = create_token_list();
		add_words(result, string, start, end);
		if (result != cmp) {
			print_list(result);
			exit(1);
		}
		exit(0);
	}
	return returnWait(pid);
}

std::pair<int, int> addSymbolAndWordTest()
{
	int testsFailed = 0;
	int testsPassed = 0;

	addWordTest("", 0, 0, {}) == false ? testsFailed++ : testsPassed++;
	addWordTest(NULL, 0, 1, {}) == false ? testsFailed++ : testsPassed++;;
	malloc_return_null();
	addWordTest("Hello", 0, 6, {}) == false ? testsFailed++ : testsPassed++;
	malloc_revert();
	addWordTest("Hello", 0, 6, {"Hello"}) == false ? testsFailed++ : testsPassed++;
	addWordTest("$%&^Hello there how are you today!@#(", 4, 33, {"Hello", "there", "how", "are", "you", "today"}) == false ? testsFailed++ : testsPassed++;

	addSymbolTest("", 0, 0, {}) == false ? testsFailed++ : testsPassed++;
	addSymbolTest(NULL, 0, 1, {}) == false ? testsFailed++ : testsPassed++;;
	malloc_return_null();
	addSymbolTest("<>||&|", 0, 8, {}) == false ? testsFailed++ : testsPassed++;
	malloc_revert();
	addSymbolTest("<>||&|", 0, 8, {"<", ">", "||", "&", "|"}) == false ? testsFailed++ : testsPassed++;
	addSymbolTest("aa<>||&|av", 2, 10, {"<", ">", "||", "&", "|"}) == false ? testsFailed++ : testsPassed++;
	addSymbolTest("<<<<<", 0, 6, {"<<<<<"}) == false ? testsFailed++ : testsPassed++;

	return {testsPassed, testsFailed};
}