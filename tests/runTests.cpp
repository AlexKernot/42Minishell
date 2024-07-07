/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runTests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:56 by akernot           #+#    #+#             */
/*   Updated: 2024/07/06 15:28:29 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

#include <iostream>
#include <vector>

std::string sanitizeString(const char *dest)
{
	if (dest == NULL) {
		return "(NULL)";
	}
	const std::size_t strlen = std::strlen(dest);
	std::string result = "";
	for (std::size_t i = 0; i < strlen; ++i) {
		if (dest[i] != '\0' && std::isprint(dest[i]) == false)
			result += '.';
		else
			result += dest[i];
	}
	return result;
}

bool returnWait(pid_t pid)
{
	int retval = 0;
	waitpid(pid, &retval, 0);
	if (WIFEXITED(retval) == true) {
		if (WEXITSTATUS(retval) != 0) {
			std::cout << "comparison failed." << std::endl;
			return false;
		}
		std::cout << "ran successfully\n";
		return true;
	}
	if (WIFSIGNALED(retval) == true) {
		std::cout << "crashed on execution. Signal: " << WCOREDUMP(retval) << std::endl;
		return false;
	}
	std::cout << "something else happened..." << std::endl;
	return false;
}

int main(int ac, char *av[])
{
	if (ac > 2) {
		std::cout << "Syntax: ./a.out [-d]\n";
		return 1;
	}
	bool debug = false;
	if (ac == 2 && std::string(av[1]) == "d") {
		debug = true;
	}
	std::vector<std::pair<int, int>> results;
	results.push_back(copyStringTest(debug));
	results.push_back(extractStringTest(debug));
	results.push_back(runTypeTests());
	
	std::cout << "\n\ncopy_string: " << results[0].first << " passed " << results[0].second << " failed.\n";
	std::cout << "extract_string: " << results[1].first << " passed " << results[1].second << " failed.\n";
	std::cout << "type tests: " << results[2].first << " passed " << results[2].second << " failed.\n";
	std::cout << "\n";
	return 0;
}