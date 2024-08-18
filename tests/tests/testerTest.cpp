/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkPipeTest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 18:13:41 by akernot           #+#    #+#             */
/*   Updated: 2024/08/04 19:24:53 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"
#include "testerTest.hpp"

#include <iostream>

#include <fcntl.h>
#include <sys/wait.h>

extern "C" {
	#include "ft_transmit.h"
	#include "ft_pipe.h"
}

basicTransmitTest::basicTransmitTest()
	: individualTest("Basic transmit test")
	{	}

basicPipeTest::basicPipeTest()
	:individualTest("Basic pipe test")
	{	}

forkPipeTest::forkPipeTest()
	: individualTest("Pipe and Fork test")
	{	}

multipleTransmitTest::multipleTransmitTest()
	: individualTest("Multiple transmits test")
	{	}

std::string makeTestName(const std::string& testName, const std::string& area)
{
	std::string res = testName + " " + area;
	return (res);
}

void basicPipeTest::test(int logFD) const
{
	int fds[2];
	if (ft_pipe(fds) != 1)
	{
		perror(makeTestName(getTestName(), "ft_pipe()").c_str());
		abort();
	}
	std::string msg = "Testhiiii";
	transmit(fds[WRITE_FD], msg.c_str());
	if (close(fds[WRITE_FD]) != 0)
	{
		perror(makeTestName(getTestName(), "close() 1").c_str());
		abort();
	}
	char *res = receive(fds[READ_FD]);
	if (close(fds[READ_FD]) != 0)
	{
		perror(makeTestName(getTestName(), "close() 2").c_str());
		abort();
	}
	fassert(logFD, std::string(res) == msg);
	free(res);
	check_mem(logFD);
}

void multipleTransmitTest::test(int logFD) const
{
	int fds[2];
	if (pipe(fds) != 0)
	{
		perror(makeTestName(getTestName(), "pipe()").c_str());
		abort();
	}
	std::string msg1 = "Test";
	std::string msg2 = "Hellooooo";
	transmit(fds[WRITE_FD], msg1.c_str());
	transmit(fds[WRITE_FD], msg2.c_str());
	close(fds[WRITE_FD]);
	char *res = receive(fds[READ_FD]);
	fassert(logFD, std::string(res) == msg1);
	free(res);
	res = receive(fds[READ_FD]);
	fassert(logFD, std::string(res) == msg2);
	free(res);
	check_mem(logFD);
}

void basicTransmitTest::test(int logFD) const
{
	int fds[2];
	if (pipe(fds) != 0)
	{
		perror(makeTestName(getTestName(), "pipe()").c_str());
		abort();
	}
	std::string message = "Test";
	transmit(fds[WRITE_FD], message.c_str());
	close(fds[WRITE_FD]);
	char *res = receive(fds[READ_FD]);
	close(fds[READ_FD]);
	fassert(logFD, std::string(res) == message);
	free(res);
	check_mem(logFD);
}

void forkPipeTest::test(int logFD) const
{
	(void)logFD;
	int pipeFDs[2];
	const int writePipe = 1;
	const int readPipe = 0;
	if (ft_pipe(pipeFDs) != 1)
	{
		perror("Test");
		abort();
	}
	pid_t pid = fork();
	if (pid == 0) {
		close(pipeFDs[readPipe]);
		std::string test = "Test";
		transmit(pipeFDs[writePipe], test.c_str());
		close(pipeFDs[writePipe]);
		exit(0);
	}
	close(pipeFDs[writePipe]);
	int result = 0;
	waitpid(pid, &result, 0);
	char *res = receive(pipeFDs[readPipe]);
	close(pipeFDs[readPipe]);
	if (res == NULL)
	{
		abort();
	}
	if (std::string(res) == "Test")
		exit(0);
	exit(1);
}

testerTestList::testerTestList()
	: fileTests("testerTest", {
		new basicTransmitTest(),
		new multipleTransmitTest(),
		new basicPipeTest(),
		new forkPipeTest()
	})
{
	
}