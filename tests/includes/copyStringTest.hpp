/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyStringTest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:21:09 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 15:43:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COPYSTRINGTEST_HPP
# define COPYSTRINGTEST_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class copyStringTest : public individualTest {
public:
	copyStringTest(const char *src, char *dest, const uint16_t start,
		const uint16_t size, std::string cmp);
	void test(int logFD) const override;
private:
	const char *src;
	char *dest;
	uint16_t start;
	uint16_t size;
	std::string cmp;
};

class copyStringTestList : public fileTests {
public:
	copyStringTestList();
private:
	char dest[10];
	std::string str;
	const char *src;
};

#endif // COPYSTRINGTEST_HPP