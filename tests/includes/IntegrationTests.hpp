/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntegrationTests.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:33:45 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 17:48:11 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTEGRATIONTESTS_HPP
# define INTEGRATIONTESTS_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

/* --- Helper functions start --- */

typedef struct	program_info
{
	int		exitVal;
	std::string	programStdout;
	std::string	programStderr;
	program_info() {
		exitVal = 0;
		programStdout = std::string();
		programStderr = std::string();
	}
}	program_info;

bool operator==(const program_info& a, const program_info& b);
bool operator!=(const program_info& a, const program_info& b);
void	createCommand(std::string toRun, std::string toStdin,
				int stdoutFD, int stderrFD);
program_info runCommand(std::string toRun, std::string toStdin);

/* --- Helper functions end --- */

class CompareShells : public individualTest
{
public:
	CompareShells(std::string command)
		: individualTest("CompareShells " + command), command(command)
		{	}
	void test(int logFD) const override;
private:
	const std::string command;
};

class IntegrationTest : public fileTests
{
public:
	IntegrationTest(std::string fileName);
private:
	std::string fileName;
};

#endif // INTEGRATIONTESTS_HPP