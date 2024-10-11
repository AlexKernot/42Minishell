/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:01:32 by akernot           #+#    #+#             */
/*   Updated: 2024/09/16 17:24:35 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "environment_variables.h"

char	*ft_strnjoin(const char *str1, const char *str2, int max_size)
{
	char	*str;
	int	size1;
	int	size2;

	size1 = ft_strlen(str1);
	size2 = ft_strlen(str2);
	if (size2 > max_size)
		size2 = max_size;
	str = (char *)malloc(sizeof(*str) * (size1 + size2 + 1));
	if (str == NULL)
		return (NULL);
	if (ft_memcpy(str, str1, size1) == NULL)
	{
		free(str);
		assert(0);
		return (NULL);
	}
	if (ft_memcpy(&str[size1], str2, size2) == NULL)
	{
		free(str);
		assert(0);
		return (NULL);
	}
	str[size1 + size2] = '\0';
	return (str);
}

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
	int	i;
	char	a;

	i = start;
	while (str[i] != '\0')
	{
		a = str[i];
		if (a == ' ' || a == '|' || a == '<' || a == '>' || a == '\''
			|| a == '\"' || a == '$')
			return (i);
		++i;
	}
	return (i);
}

char	*ft_strndup(const char *str, const size_t n)
{
	char	*new_string;
	size_t	i;
	size_t	size;

	i = 0;
	if (*str == 0)
		return (ft_calloc(1, 1));
	size = ft_strlen(str);
	new_string = (char *)malloc(size * sizeof(char) + 1);
	if (new_string == NULL || size < 1)
		return (NULL);
	while (i < size && str[i] != 0 && i < n)
	{
		new_string[i] = str[i];
		i++;
	}
	new_string[i] = 0;
	return (new_string);
}

static char *create_exit_status(const char *str, const int last_return)
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
	return retval;
}

char	*expand_str(const char *str, const char *result,
			const size_t start, const int last_rtn)
{
	size_t	symbol_pos;
	int	delim_pos;
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
	int	total;
	int	next;
	char	*result;
	char	*temp;

	total = 0;
	next = 0;
	result = NULL;
	while (total < (int)ft_strlen(str))
	{
		next = find_dollar_symbol(str, total);
		if (next == -1)
			break;
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
