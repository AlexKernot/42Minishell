/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extractStringTests.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 17:14:52 by akernot           #+#    #+#             */
/*   Updated: 2024/08/25 17:20:53 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"
#include "extractStringTests.hpp"

#include <iostream>
#include <sstream>

#include <unistd.h>
#include <stdlib.h>

extern "C" {
	#include "ft_transmit.h"
}

extern "C" char *extract_string(const char *string, const uint16_t start, const uint16_t end);

extractStringTest::extractStringTest(const char *src, const uint16_t start, const uint16_t end, std::string cmp)
	: individualTest("extract_string()")
{
	this->src = src;
	this->start = start;
	this->end = end;
	this->cmp = cmp;
}

static void transmitError(int logFD, std::string destStr, std::string cmp,
	const uint16_t start, const uint16_t end)
{
	std::stringstream str;
	str << "extract_string() failed. Expected " << cmp << " Recieved "
	<< destStr << " (" << start << "," << end << ")";
	transmit(logFD, str.str().c_str());
}

void extractStringTest::test(pid_t logFD) const
{
	const char *dest = extract_string(src, start, end);
	std::string destString = sanitizeString(dest);
	if (destString == cmp)
		exit(0);
	transmitError(logFD, destString, cmp, start, end);
	exit(1);
}

extractStringTestList::extractStringTestList()
	: fileTests("extractStringTests")
{
	testStr = "Hello";
	src = testStr.c_str();
	registerTest(new extractStringTest(NULL, 0, 1, "(NULL)"));
	registerTest(new extractStringTest(src, 1, 0, "(NULL)"));
	registerTest(new extractStringTest(src, 3, 1, "(NULL)"));
	registerTest(new extractStringTest(src, 10000, 10001, "(NULL)"));
	registerTest(new extractStringTest(src, 2, 2, ""));
	registerTest(new extractStringTest(src, 0, 1, "H"));
	registerTest(new extractStringTest(src, 0, 5, "Hello"));
	registerTest(new extractStringTest(src, 0, 100, "Hello"));
	registerTest(new extractStringTest(src, 2, 5, "llo"));
	registerTest(new extractStringTest("H\'e\'llo", 0, 7, "Hello"));
	registerTest(new extractStringTest("H\'\"e\"\'llo", 0, 9, "H\"e\"llo"));
	registerTest(new extractStringTest("H\"'e'\"llo", 0, 9, "H'e'llo"));
	registerTest(new extractStringTest("H\'e\'\"l\"lo", 0, 10, "Hello"));
	
}
