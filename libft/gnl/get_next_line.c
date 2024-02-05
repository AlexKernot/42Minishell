/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:13:59 by akernot           #+#    #+#             */
/*   Updated: 2023/10/26 14:26:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void	ft_memset(void *s, int c, size_t n)
{
	unsigned char		*pointer;
	size_t				i;

	i = 0;
	pointer = (unsigned char *)s;
	while (i < n)
	{
		pointer[i] = c;
		i++;
	}
}

char	*get_after_newline(char *string)
{
	char	*excess_string;
	int		used;
	int		excess;
	int		length;

	used = 0;
	while (string[used] != '\n' && string[used] != '\0')
		used++;
	if (string[used] == '\0')
		return (NULL);
	used++;
	length = 0;
	while (string[used + length] != '\0')
		length++;
	excess_string = (char *)gnl_calloc(length + 1, 1);
	excess = 0;
	while (string[used + excess] != '\0')
	{
		excess_string[excess] = string[used + excess];
		excess++;
	}
	return (excess_string);
}

char	*newline_in_buffer(char **full_string, char **buffer)
{
	char	*temp_string;

	temp_string = gnl_strjoin(*full_string, "\0");
	*buffer = get_after_newline(*full_string);
	free(*full_string);
	*full_string = temp_string;
	return (temp_string);
}

void	read_line(int fd, char **full_string, char **buffer, int read_return)
{
	char	*temp_string;

	while (1)
	{
		if (read_return < 1)
		{
			free(*buffer);
			*buffer = NULL;
			return ;
		}
		temp_string = gnl_strjoin(*full_string, *buffer);
		free(*full_string);
		*full_string = temp_string;
		if (check_newline(*buffer) == 1 || read_return != BUFFER_SIZE)
		{
			temp_string = get_after_newline(*buffer);
			free (*buffer);
			*buffer = temp_string;
			return ;
		}
		ft_memset(*buffer, '\0', BUFFER_SIZE);
		read_return = read(fd, *buffer, BUFFER_SIZE);
	}
}

char	*get_next_line(int fd)
{
	static char		*current_read = NULL;
	char			*full_string;
	int				read_return;

	full_string = current_read;
	if (full_string == NULL)
		full_string = (char *)gnl_calloc(1, 1);
	if (check_newline(current_read) == 1)
		return (newline_in_buffer(&full_string, &current_read));
	current_read = (char *)gnl_calloc(BUFFER_SIZE + 1, 1);
	if (current_read == NULL)
		return (NULL);
	read_return = read(fd, current_read, BUFFER_SIZE);
	if (read_return == -1 || (read_return == 0 && full_string[0] == 0))
	{
		free(full_string);
		free(current_read);
		current_read = NULL;
		return (NULL);
	}
	read_line(fd, &full_string, &current_read, read_return);
	return (full_string);
}
