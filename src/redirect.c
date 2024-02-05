/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:24:46 by akernot           #+#    #+#             */
/*   Updated: 2024/02/02 00:27:29 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"
#include "segment.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * @author Alex Kernot 
 * @brief Attempts to the name for a new temporary file by repeatedly checking
 * names until a file with that name doesn't already exist. The name generated
 * is based on the process id.
 * @return the name of a unique file that doesn't already exist.
*/
char	*create_tmp(void)
{
	char	*tmp;
	char	*string;
	int		strlen;

	tmp = ft_itoa(getpid());
	string = merge("/tmp/minishell_", tmp, "_000");
	free(tmp);
	strlen = ft_strlen(string);
	while (access(string, F_OK) != -1)
	{
		if (string[strlen - 1] == 127)
		{
			string[strlen - 1] = '0';
			++string[strlen - 2];
		}
		else
			++string[strlen - 1];
	}
	return (string);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * Create a new temporary file and write from stdin to that file until delim is
 * read. That file is then reopened in read mode and then immediately deleted.
 * The file wont be deleted until the last FD is closed and can be read from
 * freely. The file descriptor is returned.
*/
static int	delim(char	*toFind)
{
	const char	*tmp_file = create_tmp();
	char		*current_line;
	int			fd;

	fd = open(tmp_file, O_WRONLY | O_CREAT, 0666);
	if (fd == -1)
		return (-1);
	current_line = get_next_line(15);
	while (current_line != NULL)
	{
		if (ft_strncmp(toFind, current_line, ft_strlen(toFind)) == 0
			&& current_line[ft_strlen(toFind)] == '\n')
		{
			free(current_line);
			break ;
		}
		write(fd, current_line, ft_strlen(current_line));
		free(current_line);
		current_line = get_next_line(15);
	}
	fd = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	free((void *)tmp_file);
	return (fd);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * returns the file descriptor for a newly opened file depending on the type
 * required for redirecting.
*/
static int	open_file(char *file, t_segment_type type)
{
	if (type == redir_in)
		return (open(file, O_RDONLY));
	if (type == redir_out)
		return (open(file, O_WRONLY | O_CREAT, 0666));
	if (type == redir_append)
		return (open(file, O_WRONLY | O_CREAT | O_APPEND, 0666));
	return (delim(file));
}

/**
 * @author Alex Kernot
 * @brief reads the list of redirects that need to occur for a specific command.
 * Each file is opened or stdin is read. The corrosponding FDs are then
 * duplicated, ready for reading.
 * @param segment the specific command to read redirect information from
*/
int	redirect(t_segment	*segment)
{
	int				i;
	int				fd;
	t_segment_type	type;
	t_redirect		*index;

	i = 0;
	while (segment->redirects->array[i] != NULL)
	{
		index = (t_redirect *)segment->redirects->array[i];
		type = index->type;
		fd = open_file(index->file_name, type);
		if (fd == -1)
		{
			write(2, "minishell: ", 12);
			perror(index->file_name);
			return (1);
		}
		if (type == redir_in || type == redir_delim)
			dup2(fd, STDIN_FILENO);
		else if (type == redir_out || redir_append == 3)
			dup2(fd, STDOUT_FILENO);
		++i;
	}
	return (0);
}
