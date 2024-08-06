/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extractStringTests.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:56:45 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 16:12:02 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTRACTSTRINGTEST_HPP
# define EXTRACTSTRINGTEST_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class extractStringTest : public individualTest {
public:
	extractStringTest(const char *src, const uint16_t start, const uint16_t end, std::string cmp);
	void test(pid_t logFD) const override;
private:
	const char *src;
	uint16_t start;
	uint16_t end;
	std::string cmp;
};

class extractStringTestList : public fileTests {
public:
	extractStringTestList();
private:
	std::string testStr;
	const char *src;
};

#endif // EXTRACTSTRINGTEST_HPP