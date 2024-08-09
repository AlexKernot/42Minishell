/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:30:02 by akernot           #+#    #+#             */
/*   Updated: 2024/08/07 17:49:06 by akernot          ###   ########.fr       */
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
	void test(bool debug, std::string testName);
private:
	std::vector<fileTests *> testList;
};

#endif // TESTER_HPP