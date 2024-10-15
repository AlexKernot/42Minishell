/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:41:53 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:42:02 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "tokenizer.h"

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
	while (i < end && string[i] != '\0')
	{
		quote = handle_quote(string[i], quote);
		if (string[i] == ' ' && quote == '\0')
		{
			push_token(tokens, extract_string(string, j, i));
			++i;
			j = i;
			continue ;
		}
		++i;
	}
	if (j != i)
	{
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
