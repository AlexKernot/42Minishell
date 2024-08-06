/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseTests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:15:02 by akernot           #+#    #+#             */
/*   Updated: 2024/08/05 18:22:22 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parseTests.hpp"
#include "test.hpp"

#include <sstream>

extern "C" {
	#include "tokenizer.h"
	#include "ft_transmit.h"
}

static void transmitList(int logFD, const t_token_list *list, const std::vector<std::string>& cmp)
{
	std::stringstream strstr;

	strstr << "parse() test comparison failed. Expected: ";
	if (cmp.empty() == false) {
		strstr << "(NULL) ";
	}
	for (const std::string& str : cmp) {
		strstr << str << " ";
	} 
	strstr << "received: ";
	if (list == NULL) {
		strstr << "(NULL)";
		transmit(logFD, strstr.str().c_str());
		return ;
	}
	for (int i = 0; i < list->size; ++i) {
		strstr << list->array[i].content << " ";
	}
	transmit(logFD, strstr.str().c_str());
}

parseTest::parseTest(const char *string, std::vector<std::string> cmp)
	: individualTest("parse()"), string(string), cmp(cmp) 
	{	}

void parseTest::test(int logFD) const
{
	t_token_list *list = create_token_list();
	parse(list, string);
	if (list != cmp) {
		transmitList(logFD, list, cmp);
		exit(1);
	}
	delete_token_list(&list);
	check_mem(logFD);
}

parseTestList::parseTestList()
	: fileTests("parseTests", {
		new parseTest(NULL, {}),
		new parseTest("", {}),
		new parseTest("Hello", {"Hello"}),
		new parseTest("Hello There", {"Hello", "There"}),
		new parseTest("Hello <test.txt", {"Hello", "<", "test.txt"}),
		new parseTest("Hello||Test<a>b<<c", {"Hello", "||", "Test", "<", "a", ">", "b", "<<", "c"}),
		new parseTest("\"This needs to be one string\"", {"This needs to be one string"}),
		new parseTest("\'This needs to be one string\'", {"This needs to be one string"}),
		new parseTest("\"This 'needs' to be one string\"", {"This 'needs' to be one string"}),
		new parseTest("\'This \"needs\" to be one string\'", {"This \"needs\" to be one string"}),
		new parseTest("stuff<a.out\'This \"needs\" to be one string\'yess", {"stuff", "<", "a.out", "This \"needs\" to be one string", "yess"}),
		new parseTest("stuff<a.out \'This \"needs\" to be one string\' yess", {"stuff", "<", "a.out", "This \"needs\" to be one string", "yess"}),
		new parseTest("brackets! (abc) bye", {"brackets!", "(", "abc", ")", "bye"}),
		new parseTest("no combining () ?", {"no", "combining", "(", ")", "?"}),
		new parseTest("no combining++ ((hi)bye) ?", {"no", "combining++", "(", "(", "hi", ")", "bye", ")", "?"})
	})
	{	}