/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:38:13 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:39:03 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "tokenizer.h"

enum e_char_classification	get_char_type(const char a)
{
	if (ft_isascii(a) == 0 || ft_isprint(a) == 0)
		return (none_c);
	if (a == ' ')
		return (none_c);
	if (a == '&' || a == '|' || a == '>' || a == '<'
		|| a == '(' || a == ')')
		return (symbol_c);
	return (letter_c);
}

enum e_symbols	get_symbols(const char a)
{
	if (a == '<')
		return (in_s);
	if (a == '>')
		return (out_s);
	if (a == '|')
		return (pipe_s);
	if (a == '&')
		return (and_s);
	if (a == '(' || a == ')')
		return (bracket_s);
	return (none_s);
}

char	handle_quote(const char a, const char quote)
{
	if (a != '\'' && a != '\"')
		return (quote);
	if (quote == a)
		return ('\0');
	if (quote == '\0')
		return (a);
	return (quote);
}

/**
 * 
 */
void	copy_string(const char *src, char *dest,
		uint16_t start, uint16_t size)
{
	uint16_t	i;
	char		quote;
	char		a;

	if (src == NULL || dest == NULL || start >= ft_strlen(src))
		return ;
	quote = '\0';
	i = 0;
	while (i < size && src[start + i] != '\0')
	{
		a = src[start + i];
		if (a == handle_quote(a, quote)
			|| (a == quote && '\0' == handle_quote(a, quote)))
		{
			++start;
			--size;
			quote = handle_quote(a, quote);
			continue ;
		}
		quote = handle_quote(a, quote);
		dest[i] = a;
		++i;
	}
	dest[i] = '\0';
}

/**
 * Extract characters from string from start until end - 1. Ensures the
 * returned string is null terminated and doesn't copy 
 */
char	*extract_string(const char *string,
		const uint16_t start, const uint16_t end)
{
	uint16_t		size;
	char			*extract_string;

	if (string == NULL || end < start || start >= ft_strlen(string))
		return (NULL);
	size = (end - start) + 1;
	extract_string = malloc(sizeof(*extract_string) * size);
	if (extract_string == NULL)
		return (NULL);
	copy_string(string, extract_string, start, size - 1);
	return (extract_string);
}
