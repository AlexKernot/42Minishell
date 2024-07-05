/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:37 by akernot           #+#    #+#             */
/*   Updated: 2024/07/05 17:27:28 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
# define TEST_HPP

# include <string>

// helper functions used for testing
std::string sanitizeString(const char *dest);
bool returnWait(pid_t pid);

// Test functions
std::pair<int, int> copyStringTest(bool debug);
std::pair<int, int> extractStringTest(bool debug);

#endif // TEST_HPP