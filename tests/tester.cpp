/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:33:46 by akernot           #+#    #+#             */
/*   Updated: 2024/08/24 18:16:08 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.hpp"
#include "test.hpp"

# include <vector>
# include <iostream>

# include <unistd.h>

# include "fileTests.hpp"

extern "C" {
	#include "ft_pipe.h"
	#include "ft_transmit.h"
}

Tester::Tester(std::vector<fileTests *>&& tests)
{
	testList = tests;
	malloc_init();
}

void Tester::test(bool debug, std::string testName, int testIndex)
{
	std::string errorList = "";
	for (fileTests * test : testList)
	{
		const std::string errors = test->run(debug, testName, testIndex);
		errorList.append(errors);
		delete test;
	}
	if (errorList.empty() == true)
		return;
	std::cout << "\n\e[0;31mErrors:\e[0m\n\n" << errorList << std::endl;
}