/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitTest.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:34:13 by akernot           #+#    #+#             */
/*   Updated: 2024/08/20 16:39:21 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLITTEST_HPP
# define SPLITTEST_HPP

# include <string>

# include <individualTest.hpp>
# include <fileTests.hpp>

class splitTest : public individualTest
{
public:
	splitTest(const char *string, uint16_t start, std::vector<std::string> cmp)
		: individualTest("splitTest"), string(string), start(start), cmp(cmp)
		{	}
	void test(int logFD) const override;
private:
	const char *string;
	const uint16_t start;
	const std::vector<std::string> cmp;
};

class splitTestList : public fileTests
{
public:
	splitTestList();
};

#endif // SPLITTEST_HPP
