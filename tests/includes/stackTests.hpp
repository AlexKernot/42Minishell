/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stackTests.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:11:01 by akernot           #+#    #+#             */
/*   Updated: 2024/08/08 18:26:19 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKTESTS_HPP
# define STACKTESTS_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class stackCreateDestroy : public individualTest {
public:
	stackCreateDestroy()
		: individualTest("stackCreateDestroy")
		{	}
	void test(int logFD) const override;
};

class stackNullPush : public individualTest {
public:
	stackNullPush()
		: individualTest("stackNULLPush")
		{	}
	void test(int logFD) const override;
};

class stackBasicPush : public individualTest {
public:
	stackBasicPush()
		: individualTest("stackBasicPush")
		{	}
	void test(int logFD) const override;
};

class stackMultiplePush : public individualTest {
public:
	stackMultiplePush()
		: individualTest("stackMultiplePush")
		{	}
	void test(int logFD) const override;
};

class stackNullPop :public individualTest {
public:
	stackNullPop()
		: individualTest("stackNULLPop")
		{	}
	void test(int logFD) const override;
};

class stackPop : public individualTest {
public:
	stackPop()
		: individualTest("stackSinglePop")
		{	}
	void test(int logFD) const override;
};

class stackPopTooMany : public individualTest {
public:
	stackPopTooMany()
		: individualTest("stackPopTooMany")
		{	}
	void test(int logFD) const override;
};

class stackFrontNull : public individualTest {
public:
	stackFrontNull()
		: individualTest("stackFrontNull")
		{	}
	void test(int logFD) const override;
};

class stackFrontPush : public individualTest {
public:
	stackFrontPush()
		: individualTest("stackFrontPush")
		{	}
	void test(int logFD) const override;
};

class stackFrontPop : public individualTest {
public:
	stackFrontPop()
		: individualTest("stackFrontPop")
		{	}
	void test(int logFD) const override;
};

class stackTestList : public fileTests {
public:
	stackTestList();
};

#endif // STACKTESTS_HPP
