/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntegrationTests.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:30:27 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 18:25:56 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IntegrationTests.hpp"
#include "tester.hpp"
#include "test.hpp"

extern "C" {
#include "ft_pipe.h"
#include "ft_transmit.h"
}

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <sys/stat.h>
#include <filesystem>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>

extern char **environ;

static std::string lower(const std::string& a)
{
	std::string result;

	for (std::size_t i = 0; i < a.size(); ++i)
	{
		if (isalpha(a.at(i)) == false)
			result.push_back(a.at(i));
		else
			result.push_back(tolower(a.at(i)));
	}
	return result;
}

bool operator==(const program_info& a, const program_info& b)
{
	std::string cmpStdoutA = lower(a.programStdout);
	std::string cmpStdoutB = lower(b.programStdout);
	std::string cmpStderrA = lower(a.programStderr);
	std::string cmpStderrB = lower(b.programStderr);
	if (a.exitVal != b.exitVal)
		return false;
	if (cmpStdoutA != cmpStdoutB)
		return false;
	std::size_t colonA = cmpStderrA.find_last_of(':');
	if (colonA == std::string::npos)
		return (cmpStderrA == cmpStderrB);
	std::size_t colonB = cmpStderrB.find_last_of(':');
	if (colonB == std::string::npos)
		return (cmpStderrA == cmpStderrB);
	std::string errA = cmpStderrA.substr(colonA);
	std::string errB = cmpStderrB.substr(colonB);
	return (errA == errB);
}

bool operator!=(const program_info& a, const program_info& b)
{
	return !(a == b);
}

void	createCommand(std::string toRun, std::string toStdin,
				int stdoutFD, int stderrFD)
{
	char	*command = strdup(toRun.c_str());
	char	**argv = new char*[2];
	int	stdin[2];

	argv[0] = command;
	argv[1] = NULL;
	dup2(stdoutFD, STDOUT_FILENO);
	dup2(stderrFD, STDERR_FILENO);
	
	ft_pipe(stdin);
	write(stdin[WRITE_FD], toStdin.c_str(), toStdin.size());
	close(stdin[WRITE_FD]);
	dup2(stdin[READ_FD], STDIN_FILENO);
	
	chdir("./integrationTests/");
	execve(toRun.c_str(), argv, environ);
	std::string error = "test: execve: " + std::string(strerror(errno));
	write(stderrFD, error.c_str(), error.size());
	exit(1);
}

static inline void setup_poll_structure(pollfd *ready_to_read, int stdout, int stderr)
{
	ready_to_read[0].fd = stdout;
	ready_to_read[0].events = POLLIN;
	ready_to_read[0].revents = 0;
	ready_to_read[1].fd = stderr;
	ready_to_read[1].events = POLLIN;
	ready_to_read[1].revents = 0;
}

static std::string read_if_ready(pollfd& pollResult, int fd)
{
	if ((pollResult.revents&POLLIN) != POLLIN)
		return "";
	std::string result;
	FILE *file = fdopen(fd, "r");
	if (file == NULL)
		return "tester: fdopen: " + std::string(strerror(errno));
	int read_retval;
	do {
		size_t max_size = 4096;
		char *buffer = NULL;
		errno = 0;
		read_retval = getline(&buffer, &max_size, file);
		if (errno != 0)
		{
			free(buffer);
			return "tester: getline: " + std::string(strerror(errno));
		}
		result += buffer;
		free(buffer);
	} while (read_retval != -1);
	return result;
}

static inline bool setupPipe(int *fdArray, program_info& info)
{
	if (ft_pipe(fdArray) != 0)
	{
		info.exitVal = -1;
		info.programStderr = "test: ft_pipe: unknown error.";
		return false;
	}
	return true;
}

static void	interpretResults(int pid, program_info& info, int stdoutFD, int stderrFD)
{
	pollfd		ready_to_read[2];
	int 		retval = 0;

	waitpid(pid, &retval, 0);
	if (WIFEXITED(retval))
		info.exitVal = WEXITSTATUS(retval);
	else
		info.exitVal = -1;
	setup_poll_structure(ready_to_read, stdoutFD, stderrFD);
	int pollRet = poll(ready_to_read, 2, 0);
	if (pollRet == -1)
	{
		info.exitVal = -1;
		info.programStderr = "testing: poll: " + std::string(strerror(errno));
		return ;
	}
	info.programStdout = read_if_ready(ready_to_read[0], stdoutFD);
	info.programStderr = read_if_ready(ready_to_read[1], stderrFD);
}

program_info runCommand(std::string toRun, std::string toStdin)
{
	program_info	info = program_info();
	int		stdoutFDs[2];
	int		stderrFDs[2];

	
	if (setupPipe(stdoutFDs, info) == false)
		return info;
	if (setupPipe(stderrFDs, info) == false)
		return info;
	if (access("./integrationTests/outfiles", F_OK) == 0)
		std::filesystem::remove_all("./integrationTests/outfiles");
	mkdir("./integrationTests/outfiles", 0777);
	pid_t pid = fork();
	if (pid == -1)
	{
		info.exitVal = -1;
		info.programStderr = "test: fork: " + std::string(strerror(errno));
		return info;
	}
	if (pid == 0)
	{
		close(stdoutFDs[READ_FD]);
		close(stderrFDs[READ_FD]);
		createCommand(toRun, toStdin, stdoutFDs[WRITE_FD], stderrFDs[WRITE_FD]);
		exit(1);
	}
	close(stdoutFDs[WRITE_FD]);
	close(stderrFDs[WRITE_FD]);
	interpretResults(pid, info, stdoutFDs[READ_FD], stderrFDs[READ_FD]);
	return info;
}

static void transmitProgramError(int logFD, const std::string& cmd,
					const program_info& mini,
					const program_info& bash)
{
	std::stringstream str;
	str 	<< "Comparison failed: " << cmd << "\n"
		<< "mini  : " << mini.exitVal << "\n"
		<< "stdout: " << sanitizeString(mini.programStdout.c_str())
		<< "\n";
	str	<< "stderr: " << sanitizeString(mini.programStderr.c_str())
		<< "\n";
	str	<< "bash  : " << bash.exitVal << "\n"
		<< "stdout: " << sanitizeString(bash.programStdout.c_str())
		<< "\n";
	str	<< "stderr: " << sanitizeString(bash.programStderr.c_str())
		<< "\n";
	transmit(logFD, str.str().c_str());
}

void CompareShells::test(int logFD) const
{
	program_info minishellOutput = runCommand("./minishell", this->command);
	program_info bashOutput = runCommand("/bin/bash", this->command);
	if (minishellOutput != bashOutput)
	{
		transmitProgramError(logFD, this->command, minishellOutput, bashOutput);
		exit(1);
	}
	check_mem(logFD);
}

IntegrationTest::IntegrationTest(std::string fileName)
		: fileTests("Integration " + fileName), fileName(fileName)
{
	std::ifstream file;
	const std::string path = "./integrationTests/";
	file.open(path + fileName);
	while (file.eof() == false)
	{
		std::string line = "";
		std::getline(file, line);
		if (file.fail() == true)
			return ;
		if (line.empty() == true)
			continue ;
		CompareShells *test = new CompareShells(line);
		this->registerTest(test);
	}
}

/*
int main()
{
	
	tester.test(false, "", -1);
}*/
