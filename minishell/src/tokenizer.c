/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:44:40 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:19:51 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "tokenizer.h"

static int	keep_running(const char *string, int i, t_char_class type,
				char quote)
{
	const int	string_len = ft_strlen(string);

	if (i < string_len && string[i] != '\0'
		&& (get_char_type(string[i]) == type
			|| handle_quote(string[i], quote) != '\0'
			|| string[i + 1] == '\'' || string[i + 1] == '\"'))
		return (1);
	return (0);
}

uint16_t	split(t_token_list *token_list, const char *string,
	const uint16_t start, t_char_class type)
{
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
	while (keep_running(string, i, type, quote))
	{
		quote = handle_quote(string[i], quote);
		++i;
	}
	while (quote != '\0' && handle_quote(string[i], quote) == '\0')
		++i;
	if (type == letter_c)
		add_words(token_list, string, start, i);
	else if (type == symbol_c)
		add_symbols(token_list, string, start, i);
	return (i);
}

static int	find_next_split(const char *str, int *strt)
{
	t_char_class	type;
	uint16_t		i;
	char			quote;
	int				start;

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
	int				start;
	int				index;

	if (str == NULL || token_list == NULL || token_list->array == NULL)
		return ;
	start = 0;
	while (1)
	{
		index = find_next_split(str, &start);
		if (index == -1)
			return ;
		type = get_char_type(str[start]);
		start = split(token_list, str, start, type);
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
