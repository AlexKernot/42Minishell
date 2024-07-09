/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:37 by akernot           #+#    #+#             */
/*   Updated: 2024/07/09 19:09:30 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
# define TEST_HPP

# include <string>

// helper functions used for testing
std::string sanitizeString(const char *dest);
bool returnWait(pid_t pid);
void print_malloc();
void malloc_init();
void check_mem();
extern "C" void malloc_return_null();
extern "C" void malloc_revert();

// Test functions
std::pair<int, int> fakeMallocFreeTest();
std::pair<int, int> copyStringTest(bool debug);
std::pair<int, int> extractStringTest(bool debug);
std::pair<int, int> runTypeTests();
std::pair<int, int> tokenListTests();
std::pair<int, int> addSymbolAndWordTest();

#endif // TEST_HPP