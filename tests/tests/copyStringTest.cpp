/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyStringTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:44:08 by akernot           #+#    #+#             */
/*   Updated: 2024/08/25 17:21:39 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "copyStringTest.hpp"
#include "test.hpp"

#include <string>
#include <sstream>

extern "C" {
	#include "ft_transmit.h"
	#include "tokenizer.h"
}

static void transmitResult(const char *dest, const std::string& cmp,
	const uint16_t start, const uint16_t size, const int logFD)
{
	std::stringstream str;

	std::string destStr = "(NULL)";
	if (dest != NULL)
		destStr = dest;
	str << "copy_string() failed\nreceived: " << destStr << "\nexpected:";
	if (cmp.empty())
		str << "(NULL)";
	else
		str << cmp;
	str << " (" << start << "," << size << ")\n";
	transmit(logFD, str.str().c_str()); 
}

copyStringTest::copyStringTest(const char *src,
	const uint16_t start, const uint16_t size, std::string cmp)
	: individualTest("copy_string()")
{
	this->src = src;
	this->start = start;
	this->size = size;
	this->cmp = cmp;
}

void copyStringTest::test(int logFD) const
{
	char dest[25];
	memset(dest, '@', sizeof(dest));
	copy_string(src, dest, start, size);
	if ((dest[0] == '\0' || dest[0] == '@') && cmp == "")
		exit(0);
	if (dest == cmp)
		exit(0);
	transmitResult(dest, cmp, start, size, logFD);
	exit(1);
}

copyStringTestList::copyStringTestList()
	: fileTests("CopyStringTest")
{
	str = "Hello";
	src = str.c_str();
	
	registerTest(new copyStringTest(NULL, 0, 1, ""));
	registerTest(new copyStringTest(NULL, 0, 1, ""));
	registerTest(new copyStringTest(src, 0, 0, ""));
	registerTest(new copyStringTest(src, 0, 5, "Hello"));
	registerTest(new copyStringTest(src, 0, 1, "H"));
	registerTest(new copyStringTest(src, 5, 1, ""));
	registerTest(new copyStringTest(src, 38528, 1, ""));
	registerTest(new copyStringTest(src, 0, 38528, "Hello"));
	registerTest(new copyStringTest("Hello", 0, 5, "Hello"));
	registerTest(new copyStringTest("Hello", 1, 4, "ello"));
	registerTest(new copyStringTest("H\"e\"llo", 0, 7, "Hello"));
	registerTest(new copyStringTest("H\"\'e\'\"llo", 0, 9, "H\'e\'llo"));
}