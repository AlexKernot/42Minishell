/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandEnvVariableTest.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 18:06:23 by akernot           #+#    #+#             */
/*   Updated: 2024/08/15 22:54:26 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDENVVARIABLETEST_HPP
# define EXPANDENVVARIABLETEST_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class strnjoinTest : public individualTest {
public:
	strnjoinTest(const char *a, const char *b, int size, std::string expected)
		: individualTest("strnjoinTest"), a(a), b(b), size(size), expected(expected)
	{	}
	void test(int logFD) const override;
private:
	const char *a;
	const char *b;
	const int size;
	std::string expected;
};

class findDollarSignTest : public individualTest {
public:
	findDollarSignTest(const char *str, const int expected)
		: individualTest("findDollarSignTest"),
		str(str), expected(expected)
	{	}
	void test(int logFD) const override;
private:
	const char *str;
	const int expected;
};

class expandStringTest : public individualTest {
public:
	expandStringTest(const char *str, std::string expected)
		: individualTest("expandStringTest"),
			str(str), expected(expected)
	{	}
	void test(int logFD) const override;
private:
	const char *str;
	const std::string expected;
};

class envVarTest : public individualTest {
public:
	envVarTest()
		: individualTest("envVarTest")
	{	}
	void test(int logFD) const override;
};

class expandTest : public individualTest {
public:
	expandTest(const char *str, const std::string cmp);
	expandTest(const char *str, const std::string cmp,
		const int last_return);
	void test(int logFD) const override;
private:
	const char *string;
	const std::string cmp;
	const int last_return;
};

class expandTestList : public fileTests {
public:
	expandTestList();
};

#endif // EXPANDENVVARIABLETEST_HPP