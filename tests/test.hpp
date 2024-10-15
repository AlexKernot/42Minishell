/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:37 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 19:10:01 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
# define TEST_HPP

# include <string>
# include <vector>

extern "C" { 
	#include "tokenizer.h"
}

void check_mem(int logFD);

// helper functions used for testing
std::string sanitizeString(const char *dest);
void print_malloc();
void malloc_init();
void print_list(t_token_list *list, std::vector<std::string> cmp);
bool operator==(const t_token_list *a, const std::vector<std::string>& b);
bool operator!=(const t_token_list *a, const std::vector<std::string>& b);
extern "C" void malloc_return_null();
extern "C" void malloc_revert();
char *createStr(const char *text);


#endif // TEST_HPP