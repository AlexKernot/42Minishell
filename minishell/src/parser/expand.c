/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:01:32 by akernot           #+#    #+#             */
/*   Updated: 2024/08/17 18:31:47 by akernot          ###   ########.fr       */
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

/**
 * @brief Allocates a string to hold `result`, then copies at most `size` bytes
 * from `str`. Finally, if 
 */
char	*extract_and_join(const char *str, const char *result,
	const size_t size, const int last_return)
{
	char	env_str[2048];
	char	*env_var;
	char	*retval;

	if (size == 0)
		return (NULL);
	ft_memcpy(env_str, str, size);
	env_str[size] = '\0';
	if (env_str[0] == '?' && env_str[1] == '\0')
	{
		env_var = ft_itoa(last_return);
		retval = ft_strjoin(result, env_var);
		assert(retval != NULL);
		free(env_var);
	}
	else
	{
		env_var = ft_getenv(env_str);
		if (env_var == NULL)
			env_var = "";
		retval = ft_strjoin(result, env_var);
		assert(retval != NULL);
	}
	return (retval);
}

int	find_dollar_symbol(const char *str)
{
	int	i;
	char	last_quote;
	char	current;

	if (str == NULL)
		return (-1);
	last_quote = '\0';
	i = 0;
	while (str[i] != '\0')
	{
		current = str[i];
		if (current == '\'' || current == '"')
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

char	*expand_str(const char *str, const char *result, int *total,
		const int last_return)
{
	int		next;
	int		start;
	char		a;
	char		*retval;
	char		*temp;

	if (str == NULL || total == NULL)
		return (NULL);
	start = find_dollar_symbol(str) + 1;
	next = start;
	while (1)
	{
		a = str[next];
		if (a == ' ' || a == '|' || a == '<' || a == '>' || a == '\''
			|| a == '\"' || a == '\0' || a == '$')
		{
			temp = ft_strnjoin(result, str, start - 1);
			retval = extract_and_join(&str[start], temp,
				next - 1, last_return);
			free(temp);
			(*total) += next;
			return(retval);
		}
		++next;
	}
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
	while (1)
	{
		next = find_dollar_symbol(&str[total]);
		if (next == -1)
			break;
		temp = expand_str(&str[total], result, &next,
			last_return);
		total += next;
		if (result != NULL)
			free(result);
		result = temp;
	}
	if (total >= (int)ft_strlen(str))
		return (result);
	temp = ft_strnjoin(result, &str[total], (ft_strlen(str) - total));
	free(result);
	return (temp);
}
