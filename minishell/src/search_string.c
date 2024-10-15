/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:55:20 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:23:53 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "search_string.h"

#include <stddef.h>

#include "libft.h"

int	find_dollar_symbol(const char *str, size_t start)
{
	size_t	i;
	char	last_quote;
	char	current;

	if (str == NULL)
		return (-1);
	last_quote = '\0';
	i = start;
	while (i < ft_strlen(str))
	{
		current = str[i];
		if (current == '\'' || current == '\"')
		{
			if (current == last_quote)
				last_quote = '\0';
			else if (last_quote == '\0')
				last_quote = current;
		}
		if (str[i] == '$' && last_quote != '\'')
			return (i);
		++i;
	}
	return (-1);
}

int	get_delimator(const char *str, const int start)
{
	char	a;
	int		i;

	i = start;
	while (str[i] != '\0')
	{
		a = str[i];
		if (a == ' ' || a == '|' || a == '<' || a == '>' || a == '\''
			|| a == '\"' || a == '$' || a == '\n')
			return (i);
		++i;
	}
	return (i);
}
