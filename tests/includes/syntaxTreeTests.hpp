/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxTreeTests.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 18:04:15 by akernot           #+#    #+#             */
/*   Updated: 2024/08/05 18:14:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAXTREETESTS_HPP
# define SYNTAXTREETESTS_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class syntaxTreeCreateAndDestroy : public individualTest
{
public:
	syntaxTreeCreateAndDestroy()
		: individualTest("syntaxTreeCreateAndDestroy")
		{	}
	void test(int logFD) const override;
};

class syntaxTreeAddRedirect : public individualTest
{
public:
	syntaxTreeAddRedirect() 
		: individualTest("syntaxTreeAddRedirect")
		{	}
	void test(int logFD) const override;
};

class syntaxTreeAddArg : public individualTest
{
public:
	syntaxTreeAddArg()
		: individualTest("syntaxTreeAddArg")
		{	}
	void test(int logFD) const override;
};

class syntaxTreeDelete : public individualTest
{
public:
	syntaxTreeDelete()
		: individualTest("syntaxTreeDelete")
		{	}
	void test(int logFD) const override;
};

class syntaxTreeAddOp : public individualTest
{
public:
	syntaxTreeAddOp()
		: individualTest("syntaxTreeAddOp")
		{	}
	void test(int logFD) const override;
};

class syntaxTreeAddCmd : public individualTest
{
public:
	syntaxTreeAddCmd()
		: individualTest("syntaxTreeAddCmd")
		{	}
	void test(int logFD) const override;
};

class syntaxTreeTestList : public fileTests
{
public:
	syntaxTreeTestList();
};

#endif // SYNTAXTREETESTS_HPP