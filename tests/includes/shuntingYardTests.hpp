/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shuntingYardTests.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:15:55 by akernot           #+#    #+#             */
/*   Updated: 2024/08/05 19:39:38 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHUNTINGYARDTESTS_HPP
# define SHUNTINGYARDTESTS_HPP

# include <individualTest.hpp>
# include <fileTests.hpp>

class shuntingYardPurgeStack : public individualTest {
public:
	shuntingYardPurgeStack()
		: individualTest("shuntingYardPurgeStack")
		{	}
	void test(int logFD) const override;
};

class shuntingYardAddBrackets : public individualTest {
public:
	shuntingYardAddBrackets()
		: individualTest("shuntingYardAddBrackets")
		{	}
	void test(int logFD) const override;
};

class shuntingYardAddOperator : public individualTest {
public:
	shuntingYardAddOperator()
		: individualTest("shuntingYardAddOperator")
		{	}
	void test(int logFD) const override;
};

class shuntingYardAddToken : public individualTest {
public:
	shuntingYardAddToken()
		: individualTest("shuntingYardAddToken")
		{	}
	void test(int logFD) const override;
};

class shuntingYardIsOperator : public individualTest {
public:
	shuntingYardIsOperator(const char *string,
				const bool expected)
		: individualTest("shuntingYardIsOperator"),
			string(string), expected(expected)
		{	}
	void test(int logFD) const override;
private:
	const char *string;
	const bool expected;
};

class shuntingYardIsRedirect : public individualTest {
public:
	shuntingYardIsRedirect(const char *string, const bool expected)
		: individualTest("shuntingYardIsRedirect"),
			string(string), expected(expected)
		{	}
	void test(int logFD) const override;
private:
	const char *string;
	const bool expected;
};

class shuntingYardGetPrecedence : public individualTest {
public:
	shuntingYardGetPrecedence(const char *string, const int expected)
		: individualTest("shuntingYardGetPrecedence"),
			string(string), expected(expected)
		{	}
	void test(int logFD) const override;
private:
	const char *string;
	const int expected;
};

class shuntingYardConvertPostfix : public individualTest {
public:
	shuntingYardConvertPostfix(const std::vector<std::string>&& in,
			const std::vector<std::string>&& expected)
		: individualTest("shuntingYardConvertPostfix"),
		in(in), expected(expected)
		{	}
	void test(int logFD) const override;
private:
	const std::vector<std::string> in;
	const std::vector<std::string> expected;
};

class shuntingYardTestList : public fileTests {
public:
	shuntingYardTestList();
};

#endif // SHUNTINGYARDTESTS_HPP