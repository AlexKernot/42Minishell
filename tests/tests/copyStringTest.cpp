/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyStringTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:44:08 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 15:54:31 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "copyStringTest.hpp"
#include "test.hpp"

#include <string>
#include <sstream>

extern "C" {
	#include "ft_transmit.h"
}

extern "C" void	copy_string(const char *src, char *dest,
				const uint16_t start, const uint16_t size);

static void transmitResult(const char *dest, const std::string& cmp,
	const uint16_t start, const uint16_t size, const int logFD)
{
	std::stringstream str;

	std::string destStr = "(NULL)";
	if (dest != NULL)
		destStr = dest;

	str << "copy_string() failed received " << destStr << " but expected "
	<< cmp << "(" << start << "," << size << ")";
	transmit(logFD, str.str().c_str()); 
}

copyStringTest::copyStringTest(const char *src, char *dest,
	const uint16_t start, const uint16_t size, std::string cmp)
	: individualTest("copy_string()")
{
	this->src = src;
	this->dest = dest;
	this->start = start;
	this->size = size;
	this->cmp = cmp;
}

void copyStringTest::test(int logFD) const
{
	copy_string(src, dest, start, size);
	if (dest == NULL && cmp == "")
		exit(0);
	if (dest != NULL && dest == cmp)
		exit(0);
	transmitResult(dest, cmp, start, size, logFD);
	exit(1);
}

copyStringTestList::copyStringTestList()
	: fileTests("CopyStringTest")
{
	str = "Hello";
	src = str.c_str();
	
	registerTest(new copyStringTest(NULL, dest, 0, 1, ""));
	registerTest(new copyStringTest(src, NULL, 0, 1, ""));
	registerTest(new copyStringTest(NULL, NULL, 0, 1, ""));
	registerTest(new copyStringTest(src, dest, 0, 0, ""));
	registerTest(new copyStringTest(src, dest, 0, 5, "Hello"));
	registerTest(new copyStringTest(src, dest, 0, 1, "H"));
	registerTest(new copyStringTest(src, dest, 5, 1, ""));
	registerTest(new copyStringTest(src, dest, 38528, 1, ""));
	registerTest(new copyStringTest(src, dest, 0, 38528, "Hello"));
}