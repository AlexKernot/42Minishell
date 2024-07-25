/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fakeMalloc.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:29:19 by akernot           #+#    #+#             */
/*   Updated: 2024/07/25 17:18:44 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <iostream>


// A really bad malloc override for testing my program
char fake_heap[2048];
int alloc_addr[sizeof(fake_heap)];

int forceNull = 0;

void malloc_init()
{
	for (size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		alloc_addr[i] = -1;
	}
}

void print_malloc()
{
	for (size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		std::cout << (int)fake_heap[i] << ' ';
	}
	std::cout << "\n" << std::endl;
	for (size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		std::cout << (int)alloc_addr[i] << ' ';
	}
	std::cout << std::endl;
}

static char sanitize_char(int a)
{
	if (a < 32 || a > 126)
		return '.';
	return (char)a;
}

void check_mem()
{
	bool leaksDetected = false;

	for (std::size_t i = 0; i < sizeof(fake_heap); ++i)
	{
		if (alloc_addr[i] != -1) {
			if (leaksDetected == false) {
				std::cerr << "Memory leaks detected\n";
				leaksDetected = true;
			}
			int size = 1;
			for (std::size_t j = i; alloc_addr[j] == alloc_addr[i]; ++j)
				++size;
			std::cerr << "leak at " << i << " of size " << size << ":\n";
			for (std::size_t j = i; alloc_addr[j] == alloc_addr[i]; ++j)
				std::cerr << sanitize_char(fake_heap[j]);
			std::cerr << "\n";
			i += size - 1;
		}
	}
	if (leaksDetected == true)
		abort();
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
		write(STDOUT_FILENO, "null malloc\n", 12);
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
