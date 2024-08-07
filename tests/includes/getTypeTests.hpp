/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getTypeTests.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:46:28 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 17:21:15 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GETTYPETESTS_HPP
# define GETTYPETESTS_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

extern "C" {
	#include "tokenizer.h"
}

class getSymbolTest : public individualTest {
public:
	getSymbolTest(char a, t_symbols cmp);
	void test(pid_t logFD) const;
private:
	char a;
	t_symbols cmp;
};

class getWordTest : public individualTest {
public:
	getWordTest(char a, t_char_class cmp);
	void test(pid_t logFD) const;
private:
	char a;
	t_char_class cmp;
};

class getTypeTests : public fileTests {
public:
	getTypeTests();
};

#endif // GETTYPETESTS_HPP