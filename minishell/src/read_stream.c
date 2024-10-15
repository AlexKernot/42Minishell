/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stream.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:04:15 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:25:27 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "read_stream.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "libft.h"

static void	stdin_err(const char *message)
{
	if (message != NULL)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, message, ft_strlen(message));
		write(STDOUT_FILENO, "\n", 1);
	}
	exit(1);
}

static int	process_read(int read_return, char *input)
{
	if (read_return == 0)
		return (2);
	if (read_return != 1)
	{
		*input = '\0';
		perror(NULL);
		stdin_err("error reading from file");
		return (-1);
	}
	if (*input == '\n')
	{
		*input = '\0';
		return (1);
	}
	return (0);
}

int	read_stdin(char *input)
{
	static int	eof = 0;
	const int	max_command_size = 4096;
	int			write_return;
	int			i;
	int			process_ret;

	if (eof == 1 || input == NULL)
		return (1);
	i = 0;
	while (i < max_command_size)
	{
		write_return = read(STDIN_FILENO, &input[i], 1);
		process_ret = process_read(write_return, &input[i]);
		if (process_ret == -1)
			return (-1);
		if (process_ret == 2)
			eof = 1;
		if (process_ret == 2 || process_ret == 1)
			return (0);
		++i;
	}
	stdin_err("command longer than 4096 characters.");
	return (1);
}

int	is_empty(const char *input)
{
	size_t	i;
	char	a;

	i = 0;
	while (input[i] != '\0')
	{
		a = input[i];
		if (a != ' ' && a != '\n' && ft_isprint(a) == 1)
			return (0);
	}
	return (1);
}
