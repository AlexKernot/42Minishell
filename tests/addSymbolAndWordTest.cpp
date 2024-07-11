/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addSymbolAndWordTest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:46:25 by akernot           #+#    #+#             */
/*   Updated: 2024/07/11 17:01:00 by akernot          ###   ########.fr       */
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

static bool addSymbolTest(const char *string, const uint16_t start,
		const uint16_t end, std::vector<std::string> cmp)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		t_token_list *result = create_token_list();
		add_symbols(result, string, start, end);
		if (result != cmp) {
			print_list(result, cmp);
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
			print_list(result, cmp);
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
	addWordTest("No Trailing Space ", 0, 19, {"No", "Trailing", "Space"}) == false ? testsFailed++ : testsPassed++;

	addSymbolTest("", 0, 0, {}) == false ? testsFailed++ : testsPassed++;
	addSymbolTest(NULL, 0, 1, {}) == false ? testsFailed++ : testsPassed++;;
	malloc_return_null();
	addSymbolTest("<>||&|", 0, 8, {}) == false ? testsFailed++ : testsPassed++;
	malloc_revert();
	addSymbolTest("<>||&|", 0, 8, {"<", ">", "||", "&", "|"}) == false ? testsFailed++ : testsPassed++;
	addSymbolTest("aa<>||&|av", 2, 8, {"<", ">", "||", "&", "|"}) == false ? testsFailed++ : testsPassed++;
	addSymbolTest("<<<<<", 0, 6, {"<<<<<"}) == false ? testsFailed++ : testsPassed++;

	return {testsPassed, testsFailed};
}