/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:44:40 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 20:33:05 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "tokenizer.h"

/**
 * 
 */
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

void	add_words(t_token_list *tokens, const char *string,
		const uint16_t start, const uint16_t end)
{
	int		i;
	int		j;
	char	quote;

	if (tokens == NULL || tokens->array == NULL || string == NULL)
		return ;
	i = start;
	j = start;
	quote = '\0';
	//printf("\n");
	while (i < end && string[i] != '\0')
	{
		quote = handle_quote(string[i], quote);
		//printf("%c[%c]", string[i], quote);
		if (string[i] == ' ' && quote == '\0')
		{
			//printf("	Extracting '%s'\n", extract_string(string, j, i));
			push_token(tokens, extract_string(string, j, i));
			++i;
			j = i;
			continue ;
		}
		++i;
	}
	if (j != i)
	{
		//printf("\n	Expected extraction: %d %d '%s'\n", j, i, ft_substr(string, j, i - j));
		//printf("\n	Finishing Extraction '%s'\n", extract_string(string, j, i));
		push_token(tokens, extract_string(string, j, i));
	}
}

void	add_symbols(t_token_list *tokens, const char *string,
		const uint16_t start, const uint16_t end)
{
	enum e_symbols	last_symbol;
	uint16_t		i;
	uint16_t		j;

	if (tokens == NULL || string == NULL || string[0] == '\0')
		return ;
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

uint16_t	split(t_token_list *token_list, const char *string,
	const uint16_t start, t_char_class type)
{
	const size_t	string_len = ft_strlen(string);
	uint16_t		i;
	char			quote;

	quote = '\0';
	if (token_list == NULL || token_list->array == NULL || string == NULL)
		return (0);
	i = start;
	if (type == symbol_c && get_symbols(string[start]) == bracket_s)
	{
		add_symbols(token_list, string, start, i + 1);
		return (i + 1);
	}
	while (i < string_len && string[i] != '\0'
		&& (get_char_type(string[i]) == type
			|| handle_quote(string[i], quote) != '\0'
			|| string[i + 1] == '\'' || string[i + 1] == '\"'))
	{
		quote = handle_quote(string[i], quote);
		++i;
	}
	while (quote != '\0' && handle_quote(string[i], quote) == '\0')
		++i;
	//printf("	Splitting at %d: '%s'. [%s] %s [%c]\n", i, &string[start], &string[i], type == letter_c ? "Letter" : "Symbol", quote);
	if (type == letter_c)
		add_words(token_list, string, start, i);
	else if (type == symbol_c)
		add_symbols(token_list, string, start, i);
	return (i);
}

static int find_next_split(const char *str, int *strt)
{
	uint16_t	i;
	char		quote;
	t_char_class	type;
	int		start;

	if (str[*strt] == '\0')
		return (-1);
	i = *strt;
	while (str[i] == ' ')
		++i;
	*strt = i;
	start = *strt;
	quote = '\0';
	type = get_char_type(str[*strt]);
	while (i < ft_strlen(str))
	{
		quote = handle_quote(str[i], quote);
		if (type != get_char_type(str[i]) && start != i
			&& quote == '\0' && handle_quote(str[i], quote) == '\0')
		{
			return (i);
		}
		++i;
	}
	return (i);
}

void	parse(t_token_list *token_list, const char *str)
{
	t_char_class	type;
	int		start;
	int 		index;

	if (str == NULL || token_list == NULL || token_list->array == NULL)
		return ;
	start = 0;
	while (1)
	{
		index = find_next_split(str, &start);
		if (index == -1)
			return ;
		//printf("\nAbout to split %s, %d, %d, %s\n", &str[start], index, start, type == letter_c ? "Letter" : type == none_c ? "None" : "Symbol");
		type = get_char_type(str[start]);
		start = split(token_list, str, start, type);
		//printf("	type %c %s\n", str[index], type == letter_c ? "Letter" : type == none_c ? "None" : "Symbol");
	}
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
