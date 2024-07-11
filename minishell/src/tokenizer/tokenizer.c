/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:44:40 by akernot           #+#    #+#             */
/*   Updated: 2024/07/11 16:46:23 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "tokenizer.h"

/**
 * 
 */
enum e_char_classification get_char_type(const char a)
{
	if (ft_isascii(a) == 0 || ft_isprint(a) == 0)
		return (none_c);
	if (a == '\'' || a == '\"' || a == ' ')
		return (none_c);
	if (a == '&' || a == '|' || a == '>' || a == '<'
		|| a == '(' || a == ')')
		return (symbol_c);
	return (letter_c);
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
	if (a == '(' || a == ')')
		return (bracket_s);
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
	
	if (tokens == NULL || tokens->array == NULL || string == NULL)
		return ;
	i = start;
	j = start;
	while (i < end && string[i] != '\0')
 	{
		if (string[i] == ' ')
		{
			push_token(tokens, extract_string(string, j, i));
			++i;
			j = i;
			continue;
		}
		++i;
 	}
	if (j != i)
		push_token(tokens, extract_string(string, j, i));
}

void	add_symbols(t_token_list *tokens, const char *string,
		const uint16_t start, const uint16_t end)
{
	enum e_symbols	last_symbol;
	uint16_t	i;
	uint16_t	j;

	if (tokens == NULL || string == NULL || string[0] == '\0')
		return;
	last_symbol = get_symbols(string[start]);
	i = start;
	j = i;
	while (i < end && string[i] != '\0')
	{
		if (last_symbol != get_symbols(string[i]))
		{
			push_token(tokens, extract_string(string, j, i));
			last_symbol = get_symbols(string[i]);
			j = i;
		}
		++i;
	}
	if (i != j)
		push_token(tokens, extract_string(string, j, i));
}

uint16_t split(t_token_list *token_list, const char *string, const uint16_t start,
		t_char_class type)
{
	const size_t	string_len = ft_strlen(string);
	uint16_t 	i;

	if (token_list == NULL || token_list->array == NULL || string == NULL)
		return (0);
	i = start + 1;
	while (i < string_len && string[i] != '\0'
		&& get_char_type(string[i]) == (uint16_t)type)
	{
		++i;
	}
	if (type == none_c)
		return (i);
	else if (type == letter_c)
		add_words(token_list, string, start, i);
	else
		add_symbols(token_list, string, start, i);
	return (i);
}

t_char_class merge_quotes(t_token_list *token_list, const char *string,
		uint16_t *i, uint16_t *start)
{
	const uint16_t	string_len = ft_strlen(string);
	const char	quote_type = string[*i];

	if (token_list == NULL || token_list->array == NULL || string == NULL)
		return (none_c);
	split(token_list, string, *start, get_char_type(string[*start]));
	(*i)++;
	*start = *i;
	while (*i < string_len)
	{
		if (string[*i] == quote_type)
		{
			push_token(token_list, extract_string(string, *start, *i));
			(*i)++;
			(*start) = *i;
			return (get_char_type(string[*i]));
		}
		(*i)++;
	}
	return (none_c);
}

void	parse(t_token_list *token_list, const char *string)
{
	const size_t 			string_len = ft_strlen(string);
	enum e_char_classification	type;
	uint16_t			start;
	uint16_t			i;

	if (string == NULL || token_list == NULL || token_list->array == NULL)
		return;
	i = 0;
	start = 0;
	type = get_char_type(string[i]);
	while (i < string_len)
	{
		if (string[i] == '\'' || string[i] == '\"')
			type = merge_quotes(token_list, string, &i, &start);
		if (type != get_char_type(string[i]) && start != i)
		{
			i = split(token_list, string, start, type);
			type = get_char_type(string[i]);
			start = i;
		}
		++i;
	}
	if (start != i)
		split(token_list, string, start, type);
}

t_token_list	*tokenize(const char *string)
{
	t_token_list	*token_list;

	token_list = create_token_list();
	if (token_list == NULL || token_list->array == NULL)
		return (NULL);
	parse(token_list, string);
	return (token_list);
}
