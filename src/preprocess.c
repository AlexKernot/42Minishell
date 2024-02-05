/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:46:34 by akernot           #+#    #+#             */
/*   Updated: 2024/02/02 00:30:38 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @author Alex Kernot
 * STATIC:
 * reads from stdin using get next line until a closing \" or \' is found.
*/
static char	*read_stdin(char *input, char type)
{
	char	*stdin_input;
	char	*tmp;
	size_t	i;

	while (1)
	{
		write(1, "> ", 2);
		stdin_input = get_next_line(STDIN_FILENO);
		if (stdin_input == NULL)
			return (input);
		tmp = ft_strjoin(input, stdin_input);
		free (input);
		input = tmp;
		i = 0;
		while (i < ft_strlen(stdin_input))
		{
			if (stdin_input[i] == type)
			{
				free(stdin_input);
				return (input);
			}
			++i;
		}
		free(stdin_input);
	}
}

static char	*check_quotations(char *input)
{
	size_t	i;
	t_bool	open;
	char	type;

	i = 0;
	open = false;
	while (i < ft_strlen(input))
	{
		if (open == false && (input[i] == '\'' || input[i] == '\"'))
		{
			open = true;
			type = input[i];
		}
		else if (open == true && input[i] == type)
			open = false;
		++i;
	}
	if (open == true)
		return (read_stdin(input, type));
	return (input);
}

char	*preprocess(char *input)
{
	char	*new_output;

	if (input == NULL)
		return (NULL);
	if (input[0] == '\0' || input[0] == '\n')
		return (input);
	new_output = check_quotations(input);
	return (new_output);
}
