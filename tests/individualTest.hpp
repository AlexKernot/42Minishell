/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   individualTest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:05:00 by akernot           #+#    #+#             */
/*   Updated: 2024/08/07 18:04:48 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDIVIDUALTEST_HPP
# define INDIVIDUALTEST_HPP

#include <string>
#include <source_location>

# include <unistd.h>


void createError(const std::source_location& loc, int fd, const char *str, ...);
# undef fassert
# define fassert(fd, cmp) createError(std::source_location::current(), fd, #cmp, cmp);
# undef passert
# define passert(cmp) createError(std::source_location::current(), #cmp, cmp);


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
	std::string createLocationString(const std::source_location& loc);
};

#endif // INDIVIDUALTEST_HPP
