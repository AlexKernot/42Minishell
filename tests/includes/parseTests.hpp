/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseTests.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 21:13:17 by akernot           #+#    #+#             */
/*   Updated: 2024/08/04 21:33:50 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSETESTS_HPP
# define PARSETESTS_HPP

# include "individualTest.hpp"
# include "fileTests.hpp"

class parseTest : public individualTest {
public:
	parseTest(const char *string, std::vector<std::string> cmp);
	void test(int logFD) const override;
private:
	const char *string;
	std::vector<std::string> cmp;
};

class parseTestList : public fileTests {
public:
	parseTestList();
};

#endif // PARSETESTS_HPP