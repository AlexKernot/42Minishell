/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getTypeTests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:55:22 by akernot           #+#    #+#             */
/*   Updated: 2024/07/06 15:34:30 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <unistd.h>
#include <iostream>

typedef enum e_char_classification
{
	none_c,
	letter_c,
	symbol_c
} t_char_class;

typedef enum e_symbols
{
	none_s,
	in_s,
	out_s,
	pipe_s,
	and_s,
	quote_s
}	t_symbols;

bool printResult(pid_t pid)
{
	int retval = 0;
	waitpid(pid, &retval, 0);
	if (WIFEXITED(retval) == true) {
		if (WEXITSTATUS(retval) != 0) {
			return false;
		}
		return true;
	}
	if (WIFSIGNALED(retval) == true) {
		std::cout << "Crash" << std::endl;
		return false;
	}
	std::cout << "something else happened..." << std::endl;
	return false;
}

extern "C" t_symbols get_symbols(const char a);

extern "C" t_char_class	get_char_type(const char a);

bool runCharTypes(char a, t_char_class cmp)
{
	pid_t pid = fork();
	if (pid == 0) {
		t_char_class result = get_char_type(a);
		if (result != cmp) {
			std::cout << "Expected " << cmp << " got " << result << " for char " << a << std::endl;
			exit(1);
		}
		exit(0);
	}
	bool result = printResult(pid);
	return result;
}

bool runSymbolTypes(char a, t_symbols cmp)
{
	pid_t pid = fork();
	if (pid == 0) {
		t_symbols result = get_symbols(a);
		if (result != cmp) {
			std::cout << "Expected " << cmp << " got " << result << " for char " << a << std::endl;
			exit(1);
		}
		exit(0);
	}
	bool result = printResult(pid);
	return result;
}

std::pair<int, int> charType()
{
	int testsFailed = 0;
	int testsPassed = 0;
	
	std::cout << "char_class tests:" << std::endl;
	for (int i = -128; i < 128; ++i) {
		char a = (char)i;
		if (isalnum(a) == true || a == ' ')
			runCharTypes(a, letter_c) == false ? testsFailed++ : testsPassed++;
		else if (a == '<' || a == '>' || a == '|' || a == '&' || a == '\'' || a == '\"')
			runCharTypes(a, symbol_c) == false ? testsFailed++ : testsPassed++;
		else
			runCharTypes(a, none_c) == false ? testsFailed++ : testsPassed++;
	}

	return {testsPassed, testsFailed};
}

std::pair<int, int> symbolType()
{
	int testsFailed = 0;
	int testsPassed = 0;
	
	std::cout << "symbol tests:" << std::endl;
	for (int i = -128; i < 128; ++i) {
		char a = (char)i;
		if (a == '|')
			runSymbolTypes((char)i, pipe_s) == false ? testsFailed++ : testsPassed++;
		else if (a == '<')
			runSymbolTypes((char)i, in_s) == false ? testsFailed++ : testsPassed++;
		else if (a == '>')
			runSymbolTypes((char)i, out_s) == false ? testsFailed++ : testsPassed++;
		else if (a == '&')
			runSymbolTypes((char)i, and_s) == false ? testsFailed++ : testsPassed++;
		else if (a == '\"' || a == '\'')
			runSymbolTypes((char)i, quote_s) == false ? testsFailed++ : testsPassed++;
		else
			runSymbolTypes((char)i, none_s) == false ? testsFailed++ : testsPassed++;
	}
	
	return {testsPassed, testsFailed};
}

std::pair<int, int> runTypeTests()
{
	std::pair<int, int> results = charType();
	std::pair<int, int> symbolResults = symbolType();
	results.first += symbolResults.first;
	results.second += symbolResults.second;
	return results;
}
