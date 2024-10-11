/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ComplexIntegrationTest.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:59:40 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 17:46:34 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ComplexIntegrationTest.hpp"
#include "IntegrationTests.hpp"

#include <fstream>

ComplexIntegrationTest::ComplexIntegrationTest()
	: fileTests("integrationComplex", {
		new CompareShells("echo -n hello"), // No new line
		new CompareShells("export HELLO=1\necho $HELLO\nexport HELLO=2\necho $HELLO"), // Update env vars
		new CompareShells("export HELLO=1\necho $HELLO\nexport HELLO\necho $HELLO"), // Set env var to nothing
		new CompareShells("export HELLO=1\necho $HELLO\nunset HELLO\necho $HELLO"), // Unset env var
		new CompareShells("cd ..\necho $PWD $OLDPWD"), // Keep track of OLDPWD
		new CompareShells("doesntexist\n\necho $?"), // Empty line keeps retval
		new CompareShells("doesntexist\n$EMPTY\necho $?"), // Empty command clears retval
		new CompareShells("echo - "" "  " hello"), // Skip empty argument
		new CompareShells("unset USER\nunset PWD\nunset PATH\n/bin/ls"), // Unsets shouldn't break the shell
		new CompareShells("cd .. hi\npwd"), // Too many arguments to cd should do nothing
		new CompareShells("cd\npwd") // no arguments set to home
	})
{	}

HeredocIntegrationTest::HeredocIntegrationTest()
	: fileTests("integrationHeredoc")
{
	std::ifstream file;
	file.open("./integrationTests/heredoc");
	std::string buffer;
	while (file.eof() == false)
	{
		std::string line;
		std::getline(file, line);
		if (line.empty())
		{
			this->registerTest(new CompareShells(buffer));
			buffer.clear();
			continue;
		}
		buffer.append(line);
		buffer.push_back('\n');
	}
	if (buffer.empty() == false)
		this->registerTest(new CompareShells(buffer));
}