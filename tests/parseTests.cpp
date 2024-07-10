/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseTests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:24 by akernot           #+#    #+#             */
/*   Updated: 2024/07/10 16:26:09 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <iostream>
#include <vector>
#include <string>

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

static bool runParse(const char *string, std::vector<std::string> cmp)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		t_token_list *list = create_token_list();
		parse(list, string);
		if (list != cmp) {
			print_list(list);
			exit(1);
		}
		delete_token_list(&list);
		check_mem();
		exit(0);
	}
	return returnWait(pid);
}

std::pair<int, int> parseTest()
{
	int testsPassed = 0;
	int testsFailed = 0;

	runParse(NULL, {}) == false ? testsFailed++ : testsPassed++;
	runParse("", {}) == false ? testsFailed++ : testsPassed++;
	malloc_return_null();
	runParse("Hello", {}) == false ? testsFailed++ : testsPassed++;
	malloc_revert();
	runParse("Hello", {"Hello"}) == false ? testsFailed++ : testsPassed++;
	runParse("Hello There", {"Hello", "There"}) == false ? testsFailed++ : testsPassed++;
	runParse("Hello <test.txt", {"Hello", "<", "test.txt"}) == false ? testsFailed++ : testsPassed++;
	runParse("Hello||Test<a>b<<c", {"Hello", "||", "Test", "<", "a", ">", "b", "<<", "c"}) == false ? testsFailed++ : testsPassed++;

	return {testsPassed, testsFailed};
}