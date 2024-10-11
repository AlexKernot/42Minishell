/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseTests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:15:02 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 16:39:53 by akernot          ###   ########.fr       */
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

	strstr << "parse() test comparison failed.\nexpected: ";
	if (cmp.empty() == true) {
		strstr << "(NULL) ";
	}
	for (const std::string& str : cmp) {
		strstr << str << ", ";
	} 
	strstr << "\nreceived: ";
	if (list == NULL) {
		strstr << "(NULL)";
		transmit(logFD, strstr.str().c_str());
		return ;
	}
	for (int i = 0; i < list->size; ++i) {
		strstr << list->array[i] << ", ";
	}
	strstr << "\n";
	transmit(logFD, strstr.str().c_str());
}

parseTest::parseTest(const char *string, std::vector<std::string> cmp)
	: individualTest("parseTest"), string(string), cmp(cmp) 
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
		new parseTest("stuff<a.out\'This \"needs\" to be one string\'yess", {"stuff", "<", "a.outThis \"needs\" to be one stringyess"}),
		new parseTest("stuff<a.out \'This \"needs\" to be one string\' yess", {"stuff", "<", "a.out", "This \"needs\" to be one string", "yess"}),
		new parseTest("brackets! (abc) bye", {"brackets!", "(", "abc", ")", "bye"}),
		new parseTest("no combining () ?", {"no", "combining", "(", ")", "?"}),
		new parseTest("no combining++ ((hi)bye) ?", {"no", "combining++", "(", "(", "hi", ")", "bye", ")", "?"}),
		new parseTest("These\"NEED\"to\'be\'combined", {"TheseNEEDtobecombined"}),
		new parseTest("Testing\"St'r'ange\"\'Beha\"v\"iour\'Hmmmm", {"TestingSt'r'angeBeha\"v\"iourHmmmm"}),
		new parseTest("echo <\"Hello Hey There\" \"Hello Hey There\"", {"echo", "<", "Hello Hey There", "Hello Hey There"}),
		new parseTest("\"Hello     Hello\"", {"Hello     Hello"}),
		new parseTest("echo <\"../hello/\" \"Hello     Hello\"", {"echo", "<", "../hello/", "Hello     Hello"}),
		new parseTest("ls >\"./outfiles/outfile\"\"1\"\"2\"\"3\"\"4\"\"5\"", {"ls", ">", "./outfiles/outfile12345"})
	})
	{	}