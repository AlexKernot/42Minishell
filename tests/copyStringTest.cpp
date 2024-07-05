/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyStringTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:44:08 by akernot           #+#    #+#             */
/*   Updated: 2024/07/05 17:46:56 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <string>
#include <exception>
#include <iostream>

#include <sys/wait.h>
#include <unistd.h>

static void printTest(const char *src, std::string cmp,
			const uint16_t start, const uint16_t size)
{
	std::string srcString = "(NULL)";
	if (src != NULL)
		srcString = src;
	std::cout << "\nTest: " << srcString << " start " << start << " size " << size << " cmp \"" << cmp << "\"" << std::endl;
	
}

extern "C" void	copy_string(const char *src, char *dest,
					const uint16_t start, const uint16_t size);

static bool runCopyStringBypass(const char *src, char *dest,
	const uint16_t start, const uint16_t size, std::string cmp)
{
	copy_string(src, dest, start, size);
	if (dest == NULL)
		return true;
	if (dest != cmp) {
		std::cout << " comparison failed. output: " << sanitizeString(dest) << std::endl;
		return false;
	}
	std::cout << " ran successfully" << std::endl;
	return true;
}

static bool runCopyString(const char *src, char *dest,
	const uint16_t start, const uint16_t size, std::string cmp, bool debug)
{
	printTest(src, cmp, start, size);
	if (debug == true) {
		return runCopyStringBypass(src, dest, start, size, cmp);
	}
	pid_t pid = fork();
	if (pid == 0) {
		copy_string(src, dest, start, size);
		if (dest == NULL)
			exit(0);
		if (dest != cmp)
			exit(1);
		exit(0);
	}
	bool result = returnWait(pid);
	if (result == false) {
		std::cout << "Dest: " << sanitizeString(dest);
	}
	return result;
}

std::pair<int, int> copyStringTest(bool debug)
{
	std::string testString = "Hello";
	const char *src = testString.c_str();
	char *dest = new char[10];
	int testsFailed = 0;
	int testsPassed = 0;

	runCopyString(NULL, dest, 0, 1, "", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, NULL, 0, 1, "", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(NULL, NULL, 0, 1, "", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, dest, 0, 0, "", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, dest, 0, 5, "Hello", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, dest, 0, 1, "H", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, dest, 5, 1, "", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, dest, 38528, 1, "", debug) == false ? testsFailed++ : testsPassed++;
	runCopyString(src, dest, 0, 38528, "Hello", debug) == false ? testsFailed++ : testsPassed++;
	delete[] dest;
	return {testsPassed, testsFailed};
}
