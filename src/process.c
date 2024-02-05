/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 23:17:38 by akernot           #+#    #+#             */
/*   Updated: 2024/02/01 19:12:30 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "minishell.h"
#include "segment.h"
#include "run.h"
#include "environment_variables.h"
#include "redirect.h"
#include "builtin.h"

#include "libft.h"

/** 
 * @author Alex Kernot
 * STATIC:
 * defines catagories of characters so that the segment function can identify
 * where to split strings according to the capabilities of the shell.
*/
static t_char_type	catagorize(char a)
{
	if (ft_isprint(a) == false)
		return (none);
	if (a == ' ' || a == '	')
		return (space);
	if (a == '\'' || a == '\"')
		return (quotation);
	if (a == '<' || a == '>')
		return (symbol);
	return (character);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * splits the command returned from readline() into an array of single words
 * which are ready to be parsed into commands.
*/
static t_vector	*tokenizer(char *str)
{
	t_vector	*array;
	t_char_type	current;
	int			start;
	int			i;

	array = vector_ctor(25);
	current = catagorize(str[0]);
	i = 1;
	start = 0;
	while (str[i] != '\0')
	{
		if (current != catagorize(str[i]))
		{
			vector_push_back(array, ft_substr(str, start, i - start));
			current = catagorize(str[i]);
			start = i;
		}
		++i;
	}
	vector_push_back(array, ft_substr(str, start, i - start));
	return (array);
}

/**
 * @author Alex Kernot
 * @deprecated should not be used anymore. This will happen in preproccess() now
*/
static t_vector	*process_symbols(t_vector	*split, int last_return)
{
	const t_vector	*processed = vector_ctor(25);
	char			*dup;
	char			tmp;
	int				i;

	i = 0;
	while (i < split->size)
	{
		tmp = ((char *)vector_get(split, i))[0];
		if (catagorize(tmp) == quotation)
		{
			i = handle_quotation(split, processed, i);
			continue ;
		}
		if (ft_strchr((char *)vector_get(split, i), '$') != NULL)
			vector_push_back((t_vector *)processed,
				expand_env_vars((char *)vector_get(split, i), last_return));
		else if (catagorize(tmp) != space)
		{
			dup = ft_strdup(vector_get(split, i));
			vector_push_back((t_vector *)processed, dup);
		}
		++i;
	}
	return ((t_vector *)processed);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * clears all the data in a segment array and frees all the memory
*/
static void	clear_segment(t_segment *array, int len)
{
	int		i;
	int		j;
	char	**in_array;

	i = 0;
	while (i < len)
	{
		in_array = array[i].command;
		j = 0;
		while (in_array[j] != NULL)
		{
			free(in_array[j]);
			++j;
		}
		free(in_array);
		vector_dtor(&array[i].redirects);
		++i;
	}
	free (array);
}

/**
 * @author Alex Kernot
 * @brief Run new command in a new process using fork so a signal like CTRL+C can
 * kill a running command without disrupting the terminal.
 * @param input the full command to parse and run.
*/
void	run(char *input)
{
	static int	last_return = 0;
	t_vector	*split;
	t_vector	*processed;
	t_segment	*sub_commands;
	int			num_sub_commands;

	if (input == NULL || input[0] == '\0' || input[0] == '\n')
		return ;
	split = tokenizer(input);
	processed = process_symbols(split, last_return);
	vector_dtor(&split);
	sub_commands = segment((char **)processed->array);
	num_sub_commands = segment_len(sub_commands);
	if (num_sub_commands == 1)
		last_return = run_without_subshell(sub_commands);
	else
		last_return = create_subshells(sub_commands, num_sub_commands);
	vector_dtor(&processed);
	clear_segment(sub_commands, num_sub_commands);
}
