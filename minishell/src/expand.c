/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:01:32 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:57:18 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "environment_variables.h"
#include "ft_strndup.h"
#include "ft_strnjoin.h"
#include "search_string.h"

static char	*create_exit_status(const char *str, const int last_return)
{
	char	*number;
	char	*retval;

	number = ft_itoa(last_return);
	retval = ft_strjoin(number, &str[1]);
	free(number);
	return (retval);
}

char	*extract_env_var(const char *str, const size_t start,
				const size_t end, const int last_return)
{
	char		*retval;
	char		*to_expand;
	char		*env;
	uint16_t	pos;

	pos = start;
	if (str == NULL)
		return (NULL);
	if (str[start] == '$' && end == start)
		return (ft_strdup("$"));
	if (str[start] == '$')
		++pos;
	to_expand = ft_strndup(&str[pos], end - pos + 1);
	if (to_expand[0] == '?')
	{
		retval = create_exit_status(to_expand, last_return);
		free(to_expand);
		return (retval);
	}
	env = ft_getenv(to_expand);
	if (env == NULL)
		return (ft_strdup(""));
	retval = ft_strdup(env);
	free(to_expand);
	return (retval);
}

char	*expand_str(const char *str, const char *result,
			const size_t start, const int last_rtn)
{
	size_t	symbol_pos;
	int		delim_pos;
	char	*retval;
	char	*temp;
	char	*temp2;

	if (str == NULL)
		return (NULL);
	symbol_pos = find_dollar_symbol(str, start);
	delim_pos = get_delimator(str, symbol_pos + 1);
	temp = ft_strnjoin(result, &str[start], symbol_pos - start);
	temp2 = extract_env_var(str, symbol_pos, delim_pos - 1, last_rtn);
	retval = ft_strjoin(temp, temp2);
	free(temp);
	free(temp2);
	return (retval);
}

char	*expand(const char *str, const int last_return)
{
	char	*result;
	char	*temp;
	int		total;
	int		next;

	total = 0;
	next = 0;
	result = NULL;
	while (total < (int)ft_strlen(str))
	{
		next = find_dollar_symbol(str, total);
		if (next == -1)
			break ;
		temp = expand_str(str, result, total, last_return);
		if (result != NULL)
			free(result);
		result = temp;
		total = get_delimator(str, next + 1);
	}
	if (total >= (int)ft_strlen(str))
		return (result);
	temp = ft_strnjoin(result, &str[total], (ft_strlen(str) - total));
	free(result);
	return (temp);
}
