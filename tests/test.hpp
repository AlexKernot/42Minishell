/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:37 by akernot           #+#    #+#             */
/*   Updated: 2024/08/05 18:28:50 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
# define TEST_HPP

# include <string>
# include <source_location>
# include <vector>

extern "C" { 
	#include "tokenizer.h"
}

class Logger {
public:
	static void log(const std::source_location& loc, const std::string& message);
	static void flush();
private:
	Logger();
	static std::vector<std::string> logMessages;
};

void check_memory(const std::source_location& loc, int logFD);
# define check_mem(logFD) check_memory(std::source_location::current(), logFD)

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