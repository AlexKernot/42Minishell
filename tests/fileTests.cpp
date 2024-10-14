/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileTests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:04:47 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 20:21:13 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileTests.hpp"

#include <iostream>
#include <sstream>
#include <cstring>

#include <sys/ioctl.h>

extern "C" {
	#include "ft_pipe.h"
	#include "ft_transmit.h"
}

static std::string lower(const std::string& str)
{
	std::string retval;
	
	for (const char& a : str)
	{
		retval.push_back(tolower(a));
	}
	return retval;
}

fileTests::fileTests(std::string testName, std::vector<individualTest *>&& tests)
{
	name = testName;
	testList = tests;
}

fileTests::fileTests(std::string testName)
{
	name = testName;
}

void fileTests::registerTest(individualTest *test)
{
	testList.push_back(test);
}

std::string fileTests::getTestName() const
{
	return name;
}

static void execute(individualTest *test, std::string& result, bool debug)
{
	const std::string testResult = test->run(debug);
	if (testResult.empty() == true)
		return ;
	result.append(testResult);
	result.push_back('\n');
}

std::string fileTests::run(bool debug, std::string testName, int index) const
{
	write(STDOUT_FILENO, "[", 1);
	write(STDOUT_FILENO, name.c_str(), strlen(name.c_str()));
	write(STDOUT_FILENO, "]", 1);
	int spacesRequired = std::max(38 - strlen(name.c_str()), 0UL);
	for (int i = 0; i < spacesRequired; ++i)
		write(STDOUT_FILENO, " ", 1);
	std::string results = "";
	struct winsize window;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	const int resultsPerLine = std::max(window.ws_col - 40, 40);
	int i = 0;
	std::string testListName = lower(getTestName());
	for (individualTest* test : testList)
	{
		std::string nextTest = lower(test->getTestName());

		if (testName == "")
			execute(test, results, debug);
		else if ((testName == nextTest || testListName == testName)
			&& (index == i || index == -1))
			execute(test, results, debug);
		else
			write(STDOUT_FILENO, "_", 1);
		++i;
		if (i >= resultsPerLine) {
			write(STDOUT_FILENO, "\n", 1);
			for (int j = 0; j < 40; ++j)
				write(STDOUT_FILENO, " ", 1);
			i = 0;
		}
		delete test;
	}
	write(STDOUT_FILENO, "\n", 1);
	return results;
}
