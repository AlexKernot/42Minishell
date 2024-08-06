/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkPipeTest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 18:11:36 by akernot           #+#    #+#             */
/*   Updated: 2024/08/04 19:58:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTERTEST_HPP
# define TESTERTEST_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class forkPipeTest : public individualTest {
public:
	forkPipeTest();
	void test(int logFD) const override;
};

class basicPipeTest : public individualTest {
public:
	basicPipeTest();
	void test(int logFD) const override;
};

class multipleTransmitTest : public individualTest {
public:
	multipleTransmitTest();
	void test(int logFD) const override;
};

class basicTransmitTest : public individualTest {
public:
	basicTransmitTest();
	void test(int logFD) const override;
};

class testerTestList : public fileTests {
public:
	testerTestList();
};

#endif // TESTERTEST_HPP