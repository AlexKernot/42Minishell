/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:44:40 by akernot           #+#    #+#             */
/*   Updated: 2024/07/05 18:22:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "tokenizer.h"

void	add_token(t_token_list *token_list, const char *string)
{
	(void)token_list;
	(void)string;
	return ;
}

/**
 * 
 */
enum e_char_classification get_char_type(const char a)
{
	if (ft_isascii(a) == 0 || ft_isprint(a) == 0)
		return (none_c);
	if (ft_isalnum(a) || a == ' ')
		return (letter_c);
	if (a == '&' || a == '|' || a == '$' || a == '\"' || a == '\'')
		return (symbol_c);
	return (none_c);
}

enum e_symbols get_symbols(const char a)
{
	if (a == '<')
		return (in_s);
	if (a == '>')
		return (out_s);
	if (a == '|')
		return (pipe_s);
	if (a == '&')
		return (and_s);
	if (a == '\"' || a == '\'')
		return (quote_s);
	return (none_s);
	
}

/**
 * 
 */
void	copy_string(const char *src, char *dest,
		const uint16_t start, const uint16_t size)
{
	uint16_t	i;

	if (src == NULL || dest == NULL)
		return ;
	if (start >= ft_strlen(src))
		return ;
	i = 0;
	while (i < size && src[start + i] != '\0')
	{
		dest[i] = src[start + i];
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

void add_words(t_token_list *tokens, const char *string,
		const uint16_t start, const uint16_t end)
{
	int	i;
	int	j;
	
	i = start;
	j = start;
	while (i < end && string[i] != '\0')
 	{
		if (string[i] == ' ')
		{
			++i;
			add_token(tokens, extract_string(string, j, i));
			j = i;
			continue;
		}
		++i;
 	}
}

void	add_symbols(t_token_list *tokens, const char *string,
		const uint16_t start, const uint16_t end)
{
	enum e_symbols	last_symbol;
	uint16_t	i;
	uint16_t	j;

	if (tokens == NULL || string == NULL || string[0] == '\0')
		return;
	last_symbol = get_symbols(string[0]);
	i = start;
	j = i;
	while (i < end && string[i] != '\0')
	{
		if (last_symbol != get_symbols(string[i]))
		{
			++i;
			add_token(tokens, extract_string(string, j, i));
			j = i;
			continue;
		}
		++i;
	}
}

void	parse(t_token_list *token_list, const char *string)
{
	const size_t 			string_len = ft_strlen(string);
	enum e_char_classification	current_type;
	uint16_t			start;
	uint16_t			i;

	if (string == NULL || token_list == NULL)
		return;
	i = 0;
	start = 0;
	current_type = get_char_type(string[i]);
	while (i < string_len)
	{
		if (current_type != get_char_type(string[i]))
		{
			if (current_type == letter_c)
				add_words(token_list, string, start, i);
			else if (current_type == symbol_c)
				add_symbols(token_list, string, start, i);
			current_type = get_char_type(string[i]);
			start = i;
		}
		++i;
	}
}

t_token_list	*tokenize(const char *string)
{
	t_token_list	*token_list;

	token_list = malloc(sizeof(*token_list));
	if (token_list == NULL)
		return (NULL);
	token_list->array = malloc(sizeof(token_list->array) * 10);
	token_list->capacity = 10;
	token_list->length = 0;
	parse(token_list, string);
	return (token_list);
}
