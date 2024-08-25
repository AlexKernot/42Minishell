/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitTest.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:36:00 by akernot           #+#    #+#             */
/*   Updated: 2024/08/24 22:10:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <cstring>

#include "test.hpp"
#include "splitTest.hpp"

extern "C" {
	#include "token_list.h"
	#include "ft_transmit.h"
	#include "tokenizer.h"
}

static void transmitResult(int fd, std::string testName,
	t_token_list *list, const std::vector<std::string>& cmp)
{
	std::stringstream str;
	str << testName << ": Comparison failed.\nReceived: { ";
	for (uint16_t i = 0; i < list->size; ++i)
	{
		str << "\'" << list->array[i] << "\' ";
	}
	str << "}\nExpected: { ";
	for (std::size_t i = 0; i < cmp.size(); ++i)
	{
		str << "\'" << cmp[i] << "\' ";
	}
	str << "}";
	transmit(fd, str.str().c_str());
}

void splitTest::test(int logFD) const
{
	t_token_list	*list = create_token_list();
	t_char_class	type = none_c;
	if (string != NULL && start < strlen(string))
		type = get_char_type(string[start]);
	split(list, string, start, type);
	if (list != cmp)
	{
		transmitResult(logFD, getTestName(), list, cmp);
		exit(1);
	}
	delete_token_list(&list);
	check_mem(logFD);
}

splitTestList::splitTestList()
	: fileTests("splitTestList", {
		new splitTest("", 0, {}),
		new splitTest(NULL, 0, {}),
		new splitTest("AAA", 4, {}),
		new splitTest("AAAAAA", 3, {"AAA"}),
		new splitTest("Hello", 1000, {}),
		new splitTest("Hello", 0, {"Hello"}),
		new splitTest("|||AAA", 0, {"|||"}),
		new splitTest("AAA |||", 0, {"AAA"}),
		new splitTest("AAA\"AAA\"AAA", 0, {"AAAAAAAAA"}),
		new splitTest("AAA\'AAA\'AAA", 0, {"AAAAAAAAA"}),
		new splitTest("AAA\"'AAA'\"AAA", 0, {"AAA'AAA'AAA"}),
		new splitTest("AAA'\"AAA\"'AAA", 0, {"AAA\"AAA\"AAA"}),
		new splitTest("AAA\"|||\"AAA", 0, {"AAA|||AAA"}),
		new splitTest("$\"Test\"AAA", 0, {"$TestAAA"}),
		new splitTest("'This Must Be one string'", 0, {"This Must Be one string"}),
		new splitTest("\"This Must Be one string\"", 0, {"This Must Be one string"}),
		new splitTest("'This \"Must\" Be one string'", 0, {"This \"Must\" Be one string"}),
		new splitTest("\'This \"Must\" Be one string\'", 0, {"This \"Must\" Be one string"}),
		new splitTest("This'Must'Be\"One\"String", 0, {"ThisMustBeOneString"})
	})
	{	}
