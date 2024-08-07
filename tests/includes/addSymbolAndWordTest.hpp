/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addSymbolAndWordTest.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:31:57 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 15:05:48 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDSYMBOLANDWORDTEST_HPP
# define ADDSYMBOLANDWORDTEST_HPP

# include <vector>
# include <string>

# include "individualTest.hpp"
# include "fileTests.hpp"

extern "C" {
	# include "token_list.h"
}

class addXTest : public individualTest {
public:
	addXTest(std::string name, const char *input, const uint16_t start,
		const uint16_t end, std::vector<std::string> cmp);

	virtual void run(t_token_list *res, const char *input,
		const uint16_t start, const uint16_t end) const = 0;
		
	void test(pid_t logFD) const override;
private:
	const char *input;
	uint16_t start;
	uint16_t end;
	std::vector<std::string> cmp;
};

class addSymbolTest : public addXTest {
public:
	addSymbolTest(const char *input, const uint16_t start,
		const uint16_t end, std::vector<std::string> cmp);
		
	void run(t_token_list *res, const char *input,
		const uint16_t start, const uint16_t end) const override;
};

class addWordTest : public addXTest {
public:
	addWordTest(const char *input, const uint16_t start,
		const uint16_t end, std::vector<std::string> cmp);

	void run(t_token_list *res, const char *input,
		const uint16_t start, const uint16_t end) const override;
};

class addSymbolTestList : public fileTests {
public:
	addSymbolTestList();
};

class addWordTestList : public fileTests {
public:
	addWordTestList();
};

#endif // ADDSYMBOLANDWORDTEST_HPP