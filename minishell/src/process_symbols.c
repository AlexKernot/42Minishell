/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_symbols.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 23:57:27 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 16:34:30 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "environment_variables.h"
#include "redirect.h"

#include <stdlib.h>

/** 
 * @author Alex Kernot
 * STATIC:
 * merges all the elements in the vector into a single string.
 * This is used for quotation marks to turn everything into a single command.
*/
static char	*merge_vec(t_vector *to_merge)
{
	const int		vector_len = to_merge->size;
	const char		**array = (const char **)to_merge->array;
	char			*str;
	int				total_len;
	int				i;

	total_len = 0;
	i = 0;
	while (i < vector_len)
	{
		total_len += ft_strlen(array[i]);
		++i;
	}
	str = (char *)ft_calloc(sizeof(char), total_len + 1);
	i = 0;
	while (i < vector_len)
	{
		ft_memcpy(&str[ft_strlen(str)], array[i], ft_strlen(array[i]));
		++i;
	}
	return (str);
}

/**
 * @author Prachi Chawda
 * @brief Malloc a new string that replaces the environment variable named at
 * tmp and frees tmp. Fills the new string with a duplicate of the value at that
 * environment variable
 * @param tmp the string to replace
 * @param last_return if the environment variable is $?, replace it with the
 * return value of the last executed command.
*/
char	*expand_env_vars(char *tmp, int last_return)
{
	char		*env;
	char		*get;
	char		*tmp_str;
	size_t		start_env;

	start_env = (size_t)ft_strchr(tmp, '$');
	if (start_env == 0)
		return (tmp);
	start_env -= (size_t)tmp;
	if (start_env != 0)
		tmp_str = ft_substr(tmp, 0, start_env);
	else
		tmp_str = ft_strdup("");
	get = ft_substr(tmp, start_env + 1, ft_strlen(tmp) - start_env - 1);
	if (tmp != NULL && tmp[start_env] != '\0' && tmp[start_env + 1] == '?')
		env = ft_strjoin(tmp_str, ft_itoa(last_return));
	else
		env = ft_strjoin(tmp_str, ft_getenv(get));
	if (env == NULL)
		env = ft_strdup(tmp_str);
	if (get != NULL)
		free(get);
	free(tmp_str);
	return (env);
}

/**
 * @brief if a quotation mark is found, push all elements from split onto a
 * temporary vector to be merged together, before pushing the final merged
 * string onto the processed vector.
 * @param split the vector to read until a close quotation mark is found.
 * @param processed the vector to push the final result onto.
 * @param i the location in split to read from.
 * @return the index to continue reading from after this function.
*/
int	handle_quotation(const t_vector *split, const t_vector *processed, int i)
{
	t_vector	*to_merge;
	char		tmp;

	to_merge = vector_ctor(25);
	++i;
	tmp = ((char *)vector_get((t_vector *)split, i))[0];
	while (tmp != '\'' && tmp != '\"' && i < split->size)
	{
		vector_push_back(
			(t_vector *)to_merge, ft_strdup(vector_get((t_vector *)split, i)));
		++i;
		tmp = ((char *)vector_get((t_vector *)split, i))[0];
	}
	i += 2;
	vector_push_back(
		(t_vector *)processed, merge_vec((t_vector *)to_merge));
	vector_dtor(&to_merge);
	return (i);
}
