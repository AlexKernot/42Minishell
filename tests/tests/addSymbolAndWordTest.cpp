/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addSymbolAndWordTest.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:46:25 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 15:12:48 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "addSymbolAndWordTest.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include <unistd.h>

#include "individualTest.hpp"
#include "fileTests.hpp"
#include "test.hpp"

extern "C" {
	#include "tokenizer.h"	
	#include "ft_transmit.h"
}

static void transmitResult(int fd, std::string testName,
	t_token_list *list, const std::vector<std::string>& cmp)
{
	std::stringstream str;
	str << testName << ": Comparison failed.\n list { ";
	for (uint16_t i = 0; i < list->size; ++i)
	{
		str << "\'" << list->array[i].content << "\' ";
	}
	str << "}\n vec  { ";
	for (std::size_t i = 0; i < cmp.size(); ++i)
	{
		str << "\'" << cmp[i] << "\' ";
	}
	str << "}";
	transmit(fd, str.str().c_str());
}

addXTest::addXTest(std::string name, const char *input, const uint16_t start,
	const uint16_t end, std::vector<std::string> cmp)
		: individualTest(name)
{
	this->input = input;
	this->start = start;
	this->end = end;
	this->cmp = cmp;
}

void addXTest::test(pid_t logFD) const
{
	t_token_list *result = create_token_list();
	run(result, input, start, end);
	if (result == cmp)
		exit(0);
	transmitResult(logFD, getTestName(), result, cmp);
	exit(1);
}

addSymbolTest::addSymbolTest(const char *input, const uint16_t start,
	const uint16_t end, std::vector<std::string> cmp)
	: addXTest("add_symbol()", input, start, end, cmp) {	}

void addSymbolTest::run(t_token_list *res, const char *input,
	const uint16_t start, const uint16_t end) const
{
	add_symbols(res, input, start, end);
}

addWordTest::addWordTest(const char *input, const uint16_t start,
	const uint16_t end, std::vector<std::string> cmp)
	: addXTest("add_word()", input, start, end, cmp) {	}

void addWordTest::run(t_token_list *res, const char *input,
	const uint16_t start, const uint16_t end) const
{
	add_words(res, input, start, end);
}

addSymbolTestList::addSymbolTestList() : fileTests("addSymbolTest", {
	new addSymbolTest("", 0, 0, {}),
	new addSymbolTest(NULL, 0, 1, {}),
	new addSymbolTest("<>||&|", 0, 8, {"<", ">", "||", "&", "|"}),
	new addSymbolTest("aa<>||&|av", 2, 8, {"<", ">", "||", "&", "|"}),
	new addSymbolTest("<<<<<", 0, 6, {"<<<<<"})
})
{
	// No extra code needed
}

addWordTestList::addWordTestList() : fileTests("addWordTest", {
	new addWordTest("", 0, 0, {}),
	new addWordTest(NULL, 0, 1, {}),
	new addWordTest("Hello", 0, 6, {"Hello"}),
	new addWordTest("$%&^Hello there how are you today!@#(", 4, 33, {"Hello", "there", "how", "are", "you", "today"}),
	new addWordTest("No Trailing Space ", 0, 19, {"No", "Trailing", "Space"})
})
{
	// No extra code needed
}
