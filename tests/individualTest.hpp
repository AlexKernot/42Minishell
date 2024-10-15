/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   individualTest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:05:00 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 19:09:54 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDIVIDUALTEST_HPP
# define INDIVIDUALTEST_HPP

#include <string>

# include <unistd.h>


void createError(int fd, const char *str, ...);
# undef fassert
# define fassert(fd, cmp) createError(fd, #cmp, cmp);
# undef passert
# define passert(cmp) createError(#cmp, cmp);


class individualTest {
public:
	individualTest(std::string test);
	std::string run(bool debug) const;
	virtual void test(pid_t logFD) const = 0;
	std::string getTestName() const;
	virtual ~individualTest();
private:
	std::string testName;
	std::string returnWait(pid_t pid, int readFD) const;
	std::string createCrash(int retval) const;
};

#endif // INDIVIDUALTEST_HPP
