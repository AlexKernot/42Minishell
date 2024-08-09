/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runTests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:56 by akernot           #+#    #+#             */
/*   Updated: 2024/08/08 18:34:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"
#include "tester.hpp"

#include <iostream>
#include <vector>
#include <sstream>

#include <unistd.h>

#include "individualTest.hpp"

#include "testerTest.hpp"
#include "addSymbolAndWordTest.hpp"
#include "copyStringTest.hpp"
#include "extractStringTests.hpp"
#include "getTypeTests.hpp"
#include "parseTests.hpp"
#include "syntaxTreeTests.hpp"
#include "shuntingYardTests.hpp"

extern "C" {
	#include "tokenizer.h"
}

char *createStr(const char *text)
{
	char *retVal = (char *)malloc(strlen(text) + 1);
	strcpy(retVal, text);
	retVal[strlen(text)] = '\0';
	return retVal;
}

void print_list(t_token_list *list, std::vector<std::string> cmp)
{
	std::cout << "Comparing:\n{ ";
	for (uint16_t i = 0; i < list->size; ++i)
	{
		std::cout << "\'" << list->array[i].content << "\' ";
	}
	std::cout << "}\n{ ";
	for (std::size_t i = 0; i < cmp.size(); ++i)
	{
		std::cout << "\'" << cmp[i] << "\' ";
	}
	std::cout << "}" << std::endl;
}

bool operator==(const t_token_list *a, const std::vector<std::string>& b)
{
	if (a == NULL)
	{
		if (b.size() != 0)
			return false;
		return true;
	}
	if (a->size != b.size())
		return false;
	for (std::size_t i = 0; i < b.size(); ++i)
	{
		if (get_token(a, i) != b.at(i))
			return false;
	}
	return true;
}

bool operator!=(const t_token_list *a, const std::vector<std::string>& b)
{
	if (a == b)
		return false;
	return true;
}

std::string sanitizeString(const char *dest)
{
	if (dest == NULL) {
		return "(NULL)";
	}
	const std::size_t strlen = std::strlen(dest);
	std::string result = "";
	for (std::size_t i = 0; i < strlen; ++i) {
		if (dest[i] != '\0' && std::isprint(dest[i]) == false)
			result += '.';
		else
			result += dest[i];
	}
	return result;
}

static std::string lower(const std::string& str)
{
	std::string retval;
	
	for (const char& a : str)
	{
		retval.push_back(tolower(a));
	}
	return retval;
}

int main(int ac, char *av[])
{
	if (ac > 3) {
		std::cout << "Syntax: ./a.out [-d] [testName]\n";
		return 1;
	}
	bool debug = false;
	std::string test = "";
	if (ac != 1) {
		if (std::string(av[1]) == "d") {
			debug = true;
			if (av[2] != NULL)
				test = std::string(av[2]);
		} else
			test = std::string(av[1]);
	}
	test = lower(test);
	Tester tests({
		new testerTestList(),
		new addSymbolTestList(),
		new addWordTestList(),
		new copyStringTestList(),
		new extractStringTestList(),
		new getTypeTests(),
		new parseTestList(),
		new syntaxTreeTestList(),
		new shuntingYardTestList()
	});
	tests.test(debug, test);
	return 0;
}