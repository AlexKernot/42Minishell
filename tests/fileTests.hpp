/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileTests.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:05:53 by akernot           #+#    #+#             */
/*   Updated: 2024/08/24 18:13:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILETESTS_HPP
# define FILETESTS_HPP

# include <vector>
# include <string>
# include <source_location>

# include "individualTest.hpp"

class fileTests {
public:
	fileTests(std::string testName, std::vector<individualTest *>&& tests);
	fileTests(std::string testName);
	void registerTest(individualTest *test);
	std::string run(bool debug, std::string testName, int index) const;
	std::string getTestName() const;
private:
	std::string name;
	std::vector<individualTest *> testList;
};

#endif // FILETESTS_HPP
