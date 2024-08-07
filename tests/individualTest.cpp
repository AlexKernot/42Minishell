/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   individualTest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:53:56 by akernot           #+#    #+#             */
/*   Updated: 2024/08/05 20:55:34 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "individualTest.hpp"
#include "test.hpp"

#include <sstream>
#include <cstdarg>
#include <iostream>

#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

extern "C" {
	#include "ft_transmit.h"
	#include "ft_pipe.h"
} 

void createError(const std::source_location& loc, int fd, const char *str, ...)
{
	va_list args;
	va_start(args, str);
	if (va_arg(args, int) == 1) {
		va_end(args);
		return ;
	}
	std::stringstream strstr;
	strstr << "\e[0;31mAssert failed.\e[0m [" << loc.function_name() << "] "
		<< loc.file_name() << ", line " << loc.line() << ":\n	"
		<< str << "\n";
	transmit(fd, strstr.str().c_str());
	va_end(args);
	exit(1);
}

void createError(const std::source_location& loc, const char *str, ...)
{
	va_list args;
	va_start(args, str);
	if (va_arg(args, int) == 1) {
		va_end(args);
		return ;
	}
	std::stringstream strstr;
	strstr << " [" << loc.function_name() << "] "
		<< loc.file_name() << ", line " << loc.line() << ":\n	"
		<< str << "\n";
	std::cerr << strstr.str() << "\n";
	va_end(args);
	exit(1);
}

individualTest::~individualTest()
{
	
}

std::string individualTest::getTestName() const
{
	return testName;
}

individualTest::individualTest(std::string test)
{
	testName = test;
}

std::string individualTest::createLocationString(const std::source_location& loc)
{
	std::stringstream str;
	
	str << testName << " [" << loc.function_name() << "] "
		<< loc.file_name() << ", line " << loc.line() << ":\n	";
	return str.str();
}

std::string individualTest::createCrash(int retval) const
{
	std::stringstream str;
	str << "\e[1;91m";
	int signal = WTERMSIG(retval);
	str << testName << " crashed with reason " << strsignal(signal);
	if (WCOREDUMP(retval)) {
		str << " (Core dumped)";
	}
	str << "\e[0m\n";
	
	return str.str();
}

std::string individualTest::returnWait(pid_t pid, int readFD) const
{
	int retval = 0;
	waitpid(pid, &retval, WUNTRACED);
	char *failResult = receive(readFD);
	std::string str = "(NULL)";
	if (failResult != NULL)
		str = failResult;
	if (failResult != NULL)
		free(failResult);
	if (WIFEXITED(retval) == true) {
		if (WEXITSTATUS(retval) == 0) {
			write(STDOUT_FILENO, ".", 1);
			return "";
		}
		if (WEXITSTATUS(retval) == 1) {
			write(STDOUT_FILENO, "\e[0;31mF\e[0m", 13);
			return str;
		}
		if (WEXITSTATUS(retval) == 2) {
			write(STDOUT_FILENO, "\e[0;33mL\e[0m", 13);
			return str;
		}
		write(STDOUT_FILENO, "\e[1;93mO\e[0m", 13);
		return str;
	}
	if (WIFSIGNALED(retval) == true) {
		write(STDOUT_FILENO, "\e[1;91mC\e[0m", 13);
		return createCrash(retval);
	}
	write(STDOUT_FILENO, "\e[1;93mU\e[0m", 13);
	return "";
}

std::string individualTest::run() const
{
	int fds[2];
	if (ft_pipe(fds) != 1)
	{
		perror(testName.c_str());
		abort();
	}
	pid_t pid = fork();
	if (pid != 0)
	{
		passert(close(fds[WRITE_FD]) == 0)
		std::string result = returnWait(pid, fds[READ_FD]);
		passert(close(fds[READ_FD]) == 0);
		return result;
	}
	fassert(fds[WRITE_FD], close(fds[READ_FD]) == 0);
	test(fds[WRITE_FD]);
	fassert(fds[WRITE_FD], close(fds[WRITE_FD]) == 0);
	exit(0);
}
