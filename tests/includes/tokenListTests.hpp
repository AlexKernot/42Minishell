/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenListTests.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:40:06 by akernot           #+#    #+#             */
/*   Updated: 2024/08/04 21:49:56 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENLISTTEST_HPP
# define TOKENLISTTEST_HPP

# include <individualTest.hpp>
# include <fileTests.hpp>

class tokenListBasicCreateAndDelete : public individualTest {
public:
	tokenListBasicCreateAndDelete() : individualTest("token_list create & delete")
		{	}
	void test(int logFD) const override;
};

class tokenListResizeTest : public individualTest {
public:
	tokenListResizeTest() : individualTest("token_list_resize()")
		{	}
	void test(int logFD) const override;
};

class tokenListPushTokenTest : public individualTest {
public:
	tokenListPushTokenTest() : individualTest("push_token()")
		{	}
	void test(int logFD) const override;
};

class tokenListGetTokenTest : public individualTest {
public:
	tokenListGetTokenTest() : individualTest("get_token()")
		{	}
	void test(int logFD) const override;
};

class tokenListSizeTest : public individualTest {
public:
	tokenListSizeTest() : individualTest("token_list_size()")
		{	}
	void test(int logFD) const override;
};

class tokenListTestList : public fileTests {
public:
	tokenListTestList();
};

#endif
