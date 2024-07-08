/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fakeMemoryTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 18:30:16 by akernot           #+#    #+#             */
/*   Updated: 2024/07/08 13:03:35 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <iostream>

#include <unistd.h>

bool alloc(size_t size, bool expectFail)
{
	pid_t pid = fork();
	if (pid == 0) {
		char *mem = (char *)malloc(size);
		for (size_t i = 0; i < size; ++i)
		{
			mem[i] = 'a';
		}
		free(mem);
		check_mem();
		exit(0);
	}
	int result = 0;
	waitpid(pid, &result, 0);
	if (WIFEXITED(result) == true) {
		if (expectFail) {
			std::cout << "Program was supposed to fail (" << size << ")" << std::endl;
			return false;
		}
		std::cout << "Malloc and free successful" << std::endl;
		return true;
	}
	if (WIFSIGNALED(result)) {
		if (expectFail) {
			std::cout << "Malloc and free aborted as expected" << std::endl;
			return true;
		}
		std::cout << "Programm was NOT supposed to fail (" << size << ")" << std::endl;
		return false;
	}
	std::cout << "Something else happened" << std::endl;
	return false;
}

std::pair<int, int> fakeMallocFreeTest()
{
	int testsFailed = 0;
	int testsPassed = 0;
	
	alloc(1, false) == false ? testsFailed++ : testsPassed++;
	alloc(1, false) == false ? testsFailed++ : testsPassed++;
	alloc(10, false) == false ? testsFailed++ : testsPassed++;
	alloc(3000, true) == false ? testsFailed++ : testsPassed++;
	
	return {testsPassed, testsFailed};
}