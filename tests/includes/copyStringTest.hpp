/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copyStringTest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:21:09 by akernot           #+#    #+#             */
/*   Updated: 2024/08/20 14:16:40 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COPYSTRINGTEST_HPP
# define COPYSTRINGTEST_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class copyStringTest : public individualTest {
public:
	copyStringTest(const char *src, const uint16_t start,
		const uint16_t size, std::string cmp);
	void test(int logFD) const override;
private:
	const char *src;
	uint16_t start;
	uint16_t size;
	std::string cmp;
};

class copyStringTestList : public fileTests {
public:
	copyStringTestList();
private:
	std::string str;
	const char *src;
};

#endif // COPYSTRINGTEST_HPP