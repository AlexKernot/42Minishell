/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:37 by akernot           #+#    #+#             */
/*   Updated: 2024/07/21 16:42:39 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
# define TEST_HPP

# include <string>

extern "C" { 
	#include "tokenizer.h"
}

// helper functions used for testing
std::string sanitizeString(const char *dest);
bool returnWait(pid_t pid);
void print_malloc();
void malloc_init();
void check_mem();
void print_list(t_token_list *list, std::vector<std::string> cmp);
bool operator==(const t_token_list *a, const std::vector<std::string>& b);
bool operator!=(const t_token_list *a, const std::vector<std::string>& b);
extern "C" void malloc_return_null();
extern "C" void malloc_revert();
char *createStr(const char *text);

// Test functions
std::pair<int, int> fakeMallocFreeTest();
std::pair<int, int> copyStringTest(bool debug);
std::pair<int, int> extractStringTest(bool debug);
std::pair<int, int> runTypeTests();
std::pair<int, int> tokenListTests();
std::pair<int, int> addSymbolAndWordTest();
std::pair<int, int> parseTest();
std::pair<int, int> commandQueueTest();
std::pair<int, int> shuntingYardTest();

#endif // TEST_HPP