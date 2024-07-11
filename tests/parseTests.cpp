/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseTests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:24 by akernot           #+#    #+#             */
/*   Updated: 2024/07/11 17:01:33 by akernot          ###   ########.fr       */
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

static bool runParse(const char *string, std::vector<std::string> cmp)
{
	pid_t pid = fork();
	if (pid == 0)
	{
		t_token_list *list = create_token_list();
		parse(list, string);
		if (list != cmp) {
			print_list(list, cmp);
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
	runParse("\"This needs to be one string\"", {"This needs to be one string"}) == false ? testsFailed++ : testsPassed++;
	runParse("\'This needs to be one string\'", {"This needs to be one string"}) == false ? testsFailed++ : testsPassed++;
	runParse("\"This 'needs' to be one string\"", {"This 'needs' to be one string"}) == false ? testsFailed++ : testsPassed++;
	runParse("\'This \"needs\" to be one string\'", {"This \"needs\" to be one string"}) == false ? testsFailed++ : testsPassed++;
	runParse("stuff<a.out\'This \"needs\" to be one string\'yess", {"stuff", "<", "a.out", "This \"needs\" to be one string", "yess"}) == false ? testsFailed++ : testsPassed++;
	runParse("stuff<a.out \'This \"needs\" to be one string\' yess", {"stuff", "<", "a.out", "This \"needs\" to be one string", "yess"}) == false ? testsFailed++ : testsPassed++;
	runParse("brackets! (abc) bye", {"brackets!", "(", "abc", ")", "bye"}) == false ? testsFailed++ : testsPassed++;
	runParse("no combining () ?", {"no", "combining", "(", ")", "?"}) == false ? testsFailed++ : testsPassed++;
	runParse("no combining++ ((hi)bye) ?", {"no", "combining++", "(", "(", "hi", ")", "bye", ")", "?"}) == false ? testsFailed++ : testsPassed++;

	return {testsPassed, testsFailed};
}