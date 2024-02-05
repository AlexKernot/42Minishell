/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 20:49:59 by akernot           #+#    #+#             */
/*   Updated: 2024/01/29 17:44:08 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"
#include "segment.h"

/**
 * @author Alex Kernot
 * @brief counts the number of segments in an array.
 * @param input the segment array to count.
 * @return the length of the array.
*/
int	segment_len(t_segment *input)
{
	int	i;

	i = 0;
	while (input[i].command != NULL)
		++i;
	return (i);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * count how many segments are required to parse the command
*/
static int	count_segments(char **array)
{
	int		i;
	int		num;
	char	a;

	i = 0;
	num = 1;
	while (i < ft_arrlen(array))
	{
		a = array[i][0];
		if (a == '|' || a == '<' || a == '>')
			++num;
		++i;
	}
	return (num);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * This is very much a hacky work around for norminette.
 * I'm not happy with this.
*/
static void	push_new(char **input, t_segment *array, int i, int seg_i)
{
	t_redirect	*type;

	type = ft_calloc(1, sizeof(*type));
	type->type = get_type(input[i]);
	type->file_name = input[i + 1];
	vector_push_back(array[seg_i].redirects, type);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * I'm sorry this is just bad code. Norminette made it really difficult to make
 * this clean without a complete redesign.
*/
static void	create_segment(char **input, t_segment *array, int *counters)
{
	int	*i;
	int	*start;
	int	*seg_i;

	i = &counters[0];
	start = &counters[1];
	seg_i = &counters[2];
	if (get_type(input[*i]) != exec && *i > *start)
	{
		array[*seg_i].command = char_array_subset(input, *start, *i);
		*start = *i + 1;
	}
	if (input[*i][0] == '|')
	{
		++(*seg_i);
		*start = *i + 1;
		array[*seg_i].redirects = vector_ctor(10);
	}
	if (get_type(input[*i]) != exec && input[*i][0] != '|')
	{
		push_new(input, array, *i, *seg_i);
		*i += 2;
		*start = *i;
	}
}

/**
 * @author Alex Kernot
 * @brief this is the last step before commands can be run. From an array of
 * tokens, each command, argument, and redirect is grouped into its relevant
 * segment, ready to be run. This segment contains the command itself, all
 * given arguments and flags, as well as a list of files to redirect inputs and
 * outputs from.
 * @param input the seperated array of tokens.
 * @return an array of subcommands, ready to be run.
 * @remark this whole file in general is a workaround for norminette. If I were
 * to re-engineer this code, I would most likely use some sort of syntax tree
 * instead.
*/
t_segment	*segment(char **input)
{
	t_segment	*array;
	int			counters[3];
	int			*i;
	int			*start;
	int			*seg_i;

	i = &counters[0];
	start = &counters[1];
	seg_i = &counters[2];
	ft_memset(counters, 0, sizeof(counters));
	array = ft_calloc(count_segments(input) + 1, sizeof(*array));
	array->redirects = vector_ctor(10);
	while (*i < ft_arrlen(input))
	{
		create_segment(input, array, counters);
		++(*i);
	}
	if (*i <= *start || *start >= ft_arrlen(input))
		return (array);
	array[*seg_i].command = char_array_subset(input, *start, *i);
	array[*seg_i + 1].command = NULL;
	array[*seg_i + 1].redirects = NULL;
	return (array);
}
