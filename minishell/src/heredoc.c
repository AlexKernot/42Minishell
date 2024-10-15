/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:10:37 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:43:37 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "expand.h"

int	process_heredocs(t_command *command, int old_stdin, int last_return)
{
	t_redirect_type	type;
	char			*delim_string;
	int				i;
	int				fd;

	i = 0;
	while (i < command->redirects->size)
	{
		type = command->redir_types->array[i][0];
		if (type != redir_delim)
		{
			++i;
			continue ;
		}
		delim_string = command->redirects->array[i];
		fd = delim(delim_string, old_stdin, last_return);
		dup2(fd, STDIN_FILENO);
		++i;
	}
	return (0);
}

static char	*merge(char *one, char *two)
{
	char	*retval;

	if (one == NULL)
		retval = ft_strdup(two);
	else
	{
		retval = ft_strjoin(one, two);
		free(one);
	}
	return (retval);
}

static char	*read_char_by_char(int fd)
{
	static int	eof = 0;
	char		buffer[4098];
	char		*overflow;
	int			read_return;
	size_t		i;

	ft_memset(buffer, 0, sizeof(buffer));
	overflow = NULL;
	read_return = 1;
	i = 0;
	while (read_return == 1 && eof == 0)
	{
		read_return = read(fd, &buffer[i], 1);
		if (buffer[i] == '\n')
			return (merge(overflow, buffer));
		++i;
		if (i + 1 >= sizeof(buffer))
		{
			overflow = merge(overflow, buffer);
			ft_memset(buffer, 0, sizeof(buffer));
			i = 0;
		}
	}
	eof = 1;
	return (merge(overflow, buffer));
}

static int	write_to_file(char *line, int last_return, int fd)
{
	char	*expanded;

	expanded = expand(line, last_return);
	if (write(fd, expanded, ft_strlen(expanded)) == -1)
	{
		perror("minishell: HEREDOC");
		return (-1);
	}
	free(line);
	free(expanded);
	return (0);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * Create a new temporary file and write from stdin to that file until delim is
 * read. That file is then reopened in read mode and then immediately deleted.
 * The file wont be deleted until the last FD is closed and can be read from
 * freely. The file descriptor is returned.
*/
int	delim(char	*toFind, int old_stdin, int last_return)
{
	const char	*tmp_file = create_tmp();
	char		*line;
	int			fd;

	fd = open(tmp_file, O_WRONLY | O_CREAT, 0666);
	if (fd == -1)
		return (-1);
	line = read_char_by_char(old_stdin);
	while (line != NULL && line[0] != '\0')
	{
		if (isatty(old_stdin) && isatty(STDOUT_FILENO))
			write(STDOUT_FILENO, "> ", 2);
		if (ft_strncmp(toFind, line, ft_strlen(toFind)) == 0)
		{
			free(line);
			break ;
		}
		if (write_to_file(line, last_return, fd) == -1)
			return (-1);
		line = read_char_by_char(old_stdin);
	}
	fd = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	free((void *)tmp_file);
	return (fd);
}
