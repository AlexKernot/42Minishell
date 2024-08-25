/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:30:02 by akernot           #+#    #+#             */
/*   Updated: 2024/08/24 18:16:22 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_HPP
# define TESTER_HPP

# include <vector>

# include "fileTests.hpp"

class Tester
{
public:
	Tester(std::vector<fileTests *>&& tests);
	void test(bool debug, std::string testName, int testIndex);
private:
	std::vector<fileTests *> testList;
};

#endif // TESTER_HPP