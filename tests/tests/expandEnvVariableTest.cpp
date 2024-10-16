/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandEnvVariableTest.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 18:08:18 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 16:44:40 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expandEnvVariableTest.hpp"

#include "test.hpp"

#include <sstream>
#include <cstring>

extern "C" {
	#include "expand.h"
	#include "environment_variables.h"
	#include "ft_transmit.h"
}

static void setup_env_vars()
{
	char env1[12] = "HELLO=Hello";
	char env2[12] = "WORLD=World";
	char *const envp[3] = {env1, env2, NULL};
	init_env_vars(envp);
}

static std::string sanitizeFormat(const char *str)
{
	if (str == NULL)
		return "(NULL)";
	std::stringstream retval;
	for (size_t i = 0; i < strlen(str); ++i)
	{
		if (isprint(str[i]))
			retval << str[i];
		else
			retval << "\\" << std::hex << (int)str[i] << std::dec;
	}
	return (retval.str());
}

static void print(int logFD, const std::string name, const char *result,
	const char *string, std::string cmp)
{
	std::string resultStr = sanitizeFormat(result);
	std::stringstream str;
	str << name << ": Comparison failed.\n"
		<< "input:    '" << string << "'\n"
		<< "expected: '" << cmp << "'\n"
		<< "received: '" << resultStr << "'\n";
	transmit(logFD, str.str().c_str());
}

void findDollarSignTest::test(int logFD) const
{
	int result = find_dollar_symbol(str, pos);
	if (result != expected)
	{
		std::stringstream str;
		str << getTestName() << ": Comparison failed.\n"
		<< "Expected: " << expected << "\n"
		<< "received: " << result << "\n";
		transmit(logFD, str.str().c_str());
		exit(1);
	}
	check_mem(logFD);
}

void strnjoinTest::test(int logFD) const
{
	char *result = ft_strnjoin(a, b, size);
	if (result == NULL && expected.empty() == false)
	{
		print(logFD, getTestName(), result,
			(std::string(a) + std::string(b)).c_str(), expected);
		exit(1);
	} else if (std::string(result) != expected)
	{
		print(logFD, getTestName(), result,
			(std::string(a) + std::string(b)).c_str(), expected);
		exit(1);
	}
	free(result);
	check_mem(logFD);
}

expandTest::expandTest(const char *str, const std::string cmp)
	: expandTest(str, cmp, 0)
{	}

expandTest::expandTest(const char *str, const std::string cmp,
	const int last_return)
	: individualTest("expandEnvVar"), string(str),
		cmp(cmp), last_return(last_return)
	{	}

void envVarTest::test(int logFD) const
{
	setup_env_vars();
	char str1[6] = "HELLO";
	char str2[6] = "WORLD";
	fassert(logFD, std::string(ft_getenv(str1)) == std::string("Hello"));
	fassert(logFD, std::string(ft_getenv(str2)) == std::string("World"));
	clear_env_vars();
	check_mem(logFD);
}

void expandStringTest::test(int logFD) const
{
	setup_env_vars();
	int last_return = 0;
	char *result = expand_str(str, NULL, start, last_return);
	if ((result == NULL && expected.empty() == false)
		|| (result != NULL && result != expected))
	{
		print(logFD, getTestName(), result, str, expected);
		exit(1);
	}
	if (result != NULL)
		free(result);
	clear_env_vars();
	check_mem(logFD);
}

void expandTest::test(int logFD) const
{
	setup_env_vars();
	char *result = expand(string, last_return);
	if (result == NULL && cmp.find('$') != std::string::npos)
	{
		print(logFD, getTestName(), result, string, cmp);
		exit(1);
	}
	if (result != NULL && std::string(result) != cmp)
	{
		print(logFD, getTestName(), result, string, cmp);
		exit(1);
	}
	if (result != NULL)
		free(result);
	clear_env_vars();
	check_mem(logFD);
}


void extractEnvVarTest::test(int logFD) const
{
	setup_env_vars();
	char *result = extract_env_var(str, start, end, 0);
	if ((result == NULL && expected != "") || (result != expected))
	{
		print(logFD, getTestName(), result, str, expected);
		exit(1);
	}
	free(result);
	clear_env_vars();
	check_mem(logFD);
}

expandTestList::expandTestList()
: fileTests("expandEnvironmentVariableTests",
{
	new envVarTest(),
	new strnjoinTest(NULL, "", 10, ""),
	new strnjoinTest("", NULL, 10, ""),
	new strnjoinTest("", "", 10, ""),
	new strnjoinTest("a", "b", 10, "ab"),
	new strnjoinTest("Hello ", "World", 5, "Hello World"),
	new strnjoinTest("Hello ", "Worlds that don't exist", 5, "Hello World"),
	new findDollarSignTest("", 0, -1),
	new findDollarSignTest(NULL, 0, -1),
	new findDollarSignTest(" ", 0, -1),
	new findDollarSignTest("HELLO WORLD", 0, -1),
	new findDollarSignTest("$", 0, 0),
	new findDollarSignTest("\"$\"", 0, 1),
	new findDollarSignTest("'$'", 0, -1),
	new findDollarSignTest("'\"$'\"", 0, -1),
	new findDollarSignTest("\"'$'\"", 0, 2),
	new findDollarSignTest("\'a\'$", 0, 3),
	new findDollarSignTest("$$ Gogogo", 2, -1),
	new findDollarSignTest("$$$$", 0, 0),
	new expandStringTest(NULL, 0, ""),
	new extractEnvVarTest("", 0, 0, ""),
	new extractEnvVarTest("$HELLO", 0, 0, "$"),
	new extractEnvVarTest("$HELLO", 0, 5, "Hello"),
	new extractEnvVarTest("$HELLO ", 0, 5, "Hello"),
	new extractEnvVarTest("$HELLO", 0, 6, "Hello"),
	new extractEnvVarTest("$NOVAL", 0, 6, ""),
	new extractEnvVarTest("$?", 0, 1, "0"),
	new extractEnvVarTest("$?", 0, 2, "0"),
	new expandStringTest("$HELLO", 0, "Hello"),
	new expandStringTest("$WORLD", 0, "World"),
	new expandStringTest("$HELLO ", 0, "Hello"),
	new expandStringTest("$?$HELLO", 0, "0"),
	new expandStringTest("$HELLO Hihihiihi", 0, "Hello"),
	new expandStringTest("$NOVAL", 0, ""),
	new expandTest("", ""),
	new expandTest("$HELLO", "Hello"),
	new expandTest("$", "$"),
	new expandTest("$$", "$$"),
	new expandTest("$NOENV", ""),
	new expandTest("$?", "0", 0),
	new expandTest("$?", "18", 18),
	new expandTest("$HELLO$WORLD", "HelloWorld"),
	new expandTest("$?", "0"),
	new expandTest("$HELLO $WORLD", "Hello World"),
	new expandTest("$HELLO $?", "Hello 14", 14),
	new expandTest("$HELLO>$WORLD", "Hello>World"),
	new expandTest("$HELLO$WORLD$HELLO$WORLD", "HelloWorldHelloWorld"),
	new expandTest("$HELLO>$WORLD>$HELLO>$WORLD", "Hello>World>Hello>World"),
	new expandTest("$HELLO $WORLD", "Hello World"),
	new expandTest("$HELLO $WORLD $HELLO", "Hello World Hello"),
	new expandTest("$HELLO $WORLD Hii", "Hello World Hii"),
	new expandTest("$HELLO $WORLD Hii $HELLO", "Hello World Hii Hello"),
	new expandTest("$HELLO $WORLD $HELLO $HELLO", "Hello World Hello Hello"),
	new expandTest("$HELLO $WORLD $HELLO $WORLD $HELLO", "Hello World Hello World Hello"),
	new expandTest("$HELLO$WORLD", "HelloWorld"),
	new expandTest("$HELLO$?", "Hello9", 9),
	new expandTest("\"$WORLD\"", "\"World\""),
	new expandTest("\"Woooooo $HELLO wooooo\"", "\"Woooooo Hello wooooo\""),
	new expandTest("\'$HELLO $WORLD\'", "\'$HELLO $WORLD\'"),
	new expandTest("$HELLO \'$HELLO\' \"$HELLO\" $HELLO", "Hello \'$HELLO\' \"Hello\" Hello"),
	new expandTest("\'\"$HELLO\"\'", "\'\"$HELLO\"\'"),
	new expandTest("\"\'$HELLO\'\"", "\"\'Hello\'\""),
	new expandTest("\' Hiii \"$HELLO\"\'", "\' Hiii \"$HELLO\"\'"),
	new expandTest("\" Hiii \'$HELLO\'\"", "\" Hiii \'Hello\'\""),
	new expandTest("\"\'a\'\"", "\"\'a\'\"")
})
{	}