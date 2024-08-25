/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getTypeTests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:55:22 by akernot           #+#    #+#             */
/*   Updated: 2024/08/20 16:30:44 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"
#include "getTypeTests.hpp"

#include <unistd.h>
#include <iostream>
#include <sstream>

extern "C" {
	#include "tokenizer.h"
	#include "ft_transmit.h"
}

static void transmitSymbol(int logFD, char a, t_symbols res, t_symbols cmp)
{
	std::stringstream str;
	str << "get_symbol() failed. char '" << a << "' Expected "
	<< cmp << " received " << res << "\n";
	transmit(logFD, str.str().c_str());
}

static void transmitWord(int logFD, char a, t_char_class res, t_char_class cmp)
{
	std::stringstream str;
	str << "get_char_class() failed. char '" << a << "' Expected "
	<< cmp << " received " << res << "\n";
	transmit(logFD, str.str().c_str());
}

getSymbolTest::getSymbolTest(char a, t_symbols cmp)
	: individualTest("get_symbol()")
{
	this->a = a;
	this->cmp = cmp;
}

void getSymbolTest::test(pid_t logFD) const
{
	t_symbols res = get_symbols(a);
	if (res == cmp)
		exit(0);
	transmitSymbol(logFD, a, res, cmp);
	exit(1);
}

getWordTest::getWordTest(char a, t_char_class cmp)
	: individualTest("get_word()")
{
	this->a = a;
	this->cmp = cmp;
}

void getWordTest::test(pid_t logFD) const
{
	t_char_class res = get_char_type(a);
	if (res == cmp)
		exit(0);
	transmitWord(logFD, a, res, cmp);
	exit(1);
}

getTypeTests::getTypeTests()
	: fileTests("getTypeTests")
{
	for (int i = -128; i < 128; ++i) {
		char a = (char)i;
		if (a == '|')
			registerTest(new getSymbolTest((char)i, pipe_s));
		else if (a == '<')
			registerTest(new getSymbolTest((char)i, in_s));
		else if (a == '>')
			registerTest(new getSymbolTest((char)i, out_s));
		else if (a == '&')
			registerTest(new getSymbolTest((char)i, and_s));
		else if (a == '(' || a == ')')
			registerTest(new getSymbolTest((char)i, bracket_s));
		else
			registerTest(new getSymbolTest((char)i, none_s));
	}

	for (int i = -128; i < 128; ++i) {
		char a = (char)i;
		if (isprint(a) == false || isascii(a) == false || a == ' ')
			registerTest(new getWordTest(a, none_c));
		else if (a == '<' || a == '>' || a == '|' || a == '&' || a == '(' || a == ')')
			registerTest(new getWordTest(a, symbol_c));
		else
			registerTest(new getWordTest(a, letter_c));
	}
}
