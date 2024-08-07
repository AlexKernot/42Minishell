/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileTests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:04:47 by akernot           #+#    #+#             */
/*   Updated: 2024/08/05 18:41:43 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fileTests.hpp"

#include <iostream>
#include <sstream>

#include <sys/ioctl.h>

extern "C" {
	#include "ft_pipe.h"
	#include "ft_transmit.h"
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

std::string fileTests::run() const
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
	for (individualTest* test : testList)
	{
		const std::string testResult = test->run();
		delete test;
		++i;
		if (i >= resultsPerLine) {
			write(STDOUT_FILENO, "\n", 1);
			for (int j = 0; j < 40; ++j)
				write(STDOUT_FILENO, " ", 1);
			i = 0;
		}
		if (testResult.empty() == true)
			continue;
		results.append(testResult);
		results.push_back('\n');
	}
	write(STDOUT_FILENO, "\n", 1);
	return results;
}
