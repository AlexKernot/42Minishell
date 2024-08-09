/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fakeMalloc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:29:19 by akernot           #+#    #+#             */
/*   Updated: 2024/08/09 13:54:37 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <source_location>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

extern "C" {
	#include "ft_transmit.h"
}

#ifndef DEBUG

// A really bad malloc override for testing my program
char fake_heap[4096];
int alloc_addr[sizeof(fake_heap)];

int forceNull = 0;

void malloc_init()
{
	for (size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		alloc_addr[i] = -1;
	}
}

static char sanitize_char(int a)
{
	if (a < 32 || a > 126)
		return '.';
	return (char)a;
}

void check_memory(const std::source_location& loc, int logFD)
{
	bool leaksDetected = false;
	std::stringstream str;

	for (std::size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		if (alloc_addr[i] == -1) {
			continue;
		}
		if (leaksDetected == false) {
			str << "\e[0;33mMemory leaks detected at line " << loc.line()
			<< "\e[0m\n";
			leaksDetected = true;
		}
		int size = 1;
		for (std::size_t j = i; alloc_addr[j] == alloc_addr[i] && j < sizeof(fake_heap); ++j)
			++size;
		str << "\e[0;96mleak at " << i << " of size " << size << ":\n	\"";
		for (std::size_t j = i; alloc_addr[j] == alloc_addr[i] && j < sizeof(fake_heap); ++j)
			str << sanitize_char(fake_heap[j]);
		str << "\"\n\e[0m";
		i += size - 1;
	}
	if (leaksDetected == true) {
		transmit(logFD, str.str().c_str());
		exit(2);
	}
}

extern "C" {
void malloc_return_null()
{
	forceNull = 1;
}

void malloc_revert()
{
	forceNull = 0;
}

void set_allocated(size_t start, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		alloc_addr[start + i] = start;
	}
}

int is_free(size_t start, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		if (start + i >= sizeof(fake_heap))
		{
			write(2, "No space left on heap\n", 22);
			abort();
		}
		if (alloc_addr[start + i] != -1)
		{
			return (0);
		}
	}
	return 1;
}

void *malloc(size_t size)
{
	int keep_reading = 1;
	if (forceNull == 1)
	{
		forceNull = 0;
		return NULL;
	}
	for (size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		if (alloc_addr[i] == -1 && keep_reading == 1)
		{
			if (is_free(i, size) == 1)
			{
				set_allocated(i, size);
				return (fake_heap + i);
			}
			else
			{
				keep_reading = 0;
			}
		}
		else if (alloc_addr[i] != -1)
		{
			keep_reading = 1;
		}
	}
	write(2, "Malloc ran out of space.\n", 25);
	abort();
}

void free(void *ptr)
{
	if (ptr == NULL)
	{
		write(2, "Free tried freeing NULL\n", 24);
		abort();
	}
	if (ptr < fake_heap || ptr > fake_heap + sizeof(fake_heap))
	{
		write(2, "Freeing outside heap area\n", 26);
		abort();
	}
	size_t index = (uint64_t)ptr - (uint64_t)fake_heap;
	if (index > sizeof(fake_heap))
	{
		write(2, "Freeing outside area\n", 21);
		abort();
	}
	for (size_t i = 0;
		index + i <= sizeof(fake_heap) && alloc_addr[index + i] != -1;
		++i)
	{
		alloc_addr[index + i] = -1;
	}
}
}

#else

extern "C" void malloc_return_null()
{
	return ;
}

extern "C" void malloc_revert()
{
	return ;
}

void check_memory(const std::source_location& loc, int logFD)
{
	(void)loc;
	(void)logFD;
	return ;
}

void print_malloc()
{
	return ;
}

void malloc_init()
{
	return ;
}

#endif
