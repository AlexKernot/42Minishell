/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:24:46 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:10:23 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"
#include "syntax_tree.h"
#include "libft.h"

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
 * returns the file descriptor for a newly opened file depending on the type
 * required for redirecting.
*/
static int	open_file(char *file, t_redirect_type type)
{
	if (type == redir_delim)
		return (0);
	if (type == redir_in)
		return (open(file, O_RDONLY));
	if (type == redir_out)
		return (open(file, O_WRONLY | O_CREAT, 0666));
	if (type == redir_append)
		return (open(file, O_WRONLY | O_CREAT | O_APPEND, 0666));
	write(STDERR_FILENO, "minishell: unknown redirect encountered: ", 41);
	write(STDERR_FILENO, file, ft_strlen(file));
	write(STDERR_FILENO, "\n", 1);
	return (0);
}

/**
 * @author Alex Kernot
 * @brief reads the list of redirects that need to occur for a specific command.
 * Each file is opened or stdin is read. The corrosponding FDs are then
 * duplicated, ready for reading.
 * @param segment the specific command to read redirect information from
*/
int	redirect(t_command *command, int old_stdin, int last_return)
{
	t_redirect_type	type;
	char			*file;
	int				i;
	int				fd;

	i = 0;
	process_heredocs(command, old_stdin, last_return);
	while (i < command->redirects->size)
	{
		file = command->redirects->array[i];
		type = command->redir_types->array[i][0];
		fd = open_file(file, type);
		if (fd == -1)
		{
			write(2, "minishell: ", 11);
			perror(file);
			return (1);
		}
		if (type == redir_in)
			dup2(fd, STDIN_FILENO);
		else if (type == redir_out || type == redir_append)
			dup2(fd, STDOUT_FILENO);
		++i;
	}
	return (0);
}
