/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_transmit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:29:16 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:46:27 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "ft_transmit.h"

#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>

/**
 * @author Alex Kernot
 * @brief Initially writes the number of characters in message to the pipe
 * before sending the string. This allows the pipe to be read with one call to
 * read().
 * @param write_fd the file descriptor for the write end of a pipe.
 * @param message the message to write to the pipe.
*/
void	transmit(int write_fd, char *message)
{
	char	*num_to_str;
	int		num_bytes;
	int		num_strlen;

	if (message == NULL)
	{
		write(write_fd, "2\4\0", 3);
		return ;
	}
	num_bytes = ft_strlen(message) + 1;
	num_to_str = ft_itoa(num_bytes);
	num_strlen = ft_strlen(num_to_str);
	write(write_fd, num_to_str, num_strlen);
	free((void *)num_to_str);
	write(write_fd, message, num_bytes);
}

/**
 * @author Alex Kernot
 * @brief first reads how many characters have been written to this pipe, then
 * reads the message sent by transmit() in one read() call.
 * @param read_fd the file descriptor for the read end of a pipe. Data is only
 * valid if this pipe was filled with data by a call to transmit().
 * @return a newly allocated string containing the message sent by transmit,
 * exluding the characters indicating the length of the string.
*/
char	*receive(int read_fd)
{
	char	first_char;
	int		num_bytes;
	char	*string;

	if (read(read_fd, &first_char, 1) != 1)
		return (NULL);
	num_bytes = 0;
	while (ft_isdigit(first_char) == true)
	{
		num_bytes *= 10;
		num_bytes += first_char - '0';
		if (read(read_fd, &first_char, 1) != 1)
			return (NULL);
	}
	string = ft_calloc(sizeof(*string), num_bytes);
	string[0] = first_char;
	if (num_bytes - 1 == 0)
		return (string);
	read(read_fd, &(string[1]), num_bytes - 1);
	return (string);
}
