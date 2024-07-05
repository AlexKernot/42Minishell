/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extractStringTests.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 17:14:52 by akernot           #+#    #+#             */
/*   Updated: 2024/07/05 18:25:43 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <iostream>

#include <unistd.h>

bool returnNull = false;
char fakeAlloc[15] = {0};

extern "C" void *malloc(size_t size) {
	std::cout << " (Using fake malloc, null=" << returnNull << ") ";
	// clear the data in fake alloc just in case
	for (std::size_t i = 0; i < sizeof(fakeAlloc); ++i)
		fakeAlloc[i] = '\0';
	if (returnNull == true)
		return NULL;
	(void)size;
	return fakeAlloc;
}

extern "C" char *extract_string(const char *string, const uint16_t start, const uint16_t end);

static bool bypassExtractString(const char *src, const uint16_t start, const uint16_t end, std::string cmp)
{
	const char *dest = extract_string(src, start, end);
	if (dest == NULL)
		return true;
	if (dest != cmp) {
		std::cout << " comparison failed. output: " << sanitizeString(dest) << std::endl;
		return false;
	}
	std::cout << " ran successfully" << std::endl;
	return true;
}

static void printTest(const char *src, std::string cmp, const uint16_t start, const uint16_t end)
{
	std::string srcString = "(NULL)";
	if (src != NULL)
		srcString = src;
	std::cout << "Test: src " << srcString << " cmp " << cmp << " start " << start << " end " << end << std::endl;
}

static bool runExtractString(const char *src, const uint16_t start, const uint16_t end, std::string cmp, bool debug)
{
	printTest(src, cmp, start, end);
	if (debug == true) {
		return bypassExtractString(src, start, end, cmp);
	}
	pid_t pid = fork();
	if (pid == 0) {
		const char *dest = extract_string(src, start, end);
		std::string destString = sanitizeString(dest);
		if (destString != cmp) {
			std::cout << "Dest: " << sanitizeString(dest);
			exit(1);
		}
		exit(0);
	}
	bool result = returnWait(pid);
	return result;
}

std::pair<int, int> extractStringTest(bool debug)
{
	std::string testString = "Hello";
	const char *src = testString.c_str();
	int testsFailed = 0;
	int testsPassed = 0;
	runExtractString(NULL, 0, 1, "(NULL)", debug) == false ? testsFailed++ : testsPassed++;
	// Test a failed malloc
	returnNull = true;
	runExtractString(src, 0, 1, "(NULL)", debug) == false ? testsFailed++ : testsPassed++;
	returnNull = false;
	runExtractString(src, 1, 0, "(NULL)", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 3, 1, "(NULL)", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 10000, 10001, "(NULL)", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 2, 2, "", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 0, 1, "H", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 0, 5, "Hello", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 0, 100, "Hello", debug) == false ? testsFailed++ : testsPassed++;
	runExtractString(src, 2, 5, "llo", debug) == false ? testsFailed++ : testsPassed++;
	return {testsPassed, testsFailed};
}
