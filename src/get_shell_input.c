/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_shell_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 17:44:48 by akernot           #+#    #+#             */
/*   Updated: 2024/02/01 20:32:12 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "ft_transmit.h"
#include "redirect.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <signal.h>

/**
 * @author Alex Kernot
 * @brief merges 3 strings. Functions identically to string join except takes 3
 * parameters
 * @param a the first string to merge
 * @param b the second string to merge
 * @param c the second string to merge
 * @return a newly allocated string containing the contents of all 3 strings,
 * connected one after the other.
*/
char	*merge(const char *a, char *b, char *c)
{
	char	*tmp;
	char	*retval;

	tmp = ft_strjoin(a, b);
	retval = ft_strjoin(tmp, c);
	free(tmp);
	return (retval);
}

/**
 * @author Alex Kernot
 * @brief creates a string containing the user, hostname, and current working
 * directory in the format // * "USER@HOSTNAME:CWD$".
 * @param combined a string containing the user and hostname already combined.
 * @return a newly alloced string containing the prompt to display.
*/
char	*generate_prompt(const char *combined)
{
	const char	*home = getenv("HOME");
	char		cwd[4096];
	char		*removed_home;

	removed_home = NULL;
	getcwd(cwd, 4096);
	if (ft_strncmp(home, cwd, ft_strlen(home)) == 0)
	{
		if (ft_strlen(cwd) == ft_strlen(home))
			removed_home = &cwd[ft_strlen(home)];
		else
			removed_home = ft_strchr(&cwd[ft_strlen(home)], '/');
	}
	if (removed_home != NULL)
	{
		removed_home -= 1;
		*removed_home = '~';
	}
	else
		removed_home = cwd;
	return (merge(combined, removed_home, "\001\x1b[0m\002$ "));
}

/** 
 * @author Alex Kernot
 * STATIC:
 * waits for the child process running readline() to terminate and gets its
 * output. If a signal terminated the readline() process, instead do not handle
 * the command and instead display a new prompt. Environment variables and
 * quotation marks are initially processed here.
*/
static char	*get_input(int pid, int read_fd)
{
	char	*output;
	int		retval;

	retval = 0;
	waitpid(pid, &retval, 0);
	output = NULL;
	if (WIFEXITED(retval) && WEXITSTATUS(retval) == 0)
		output = receive(read_fd);
	if (output == NULL)
		printf("\n");
	if (output != NULL && output[0] == '\4')
	{
		printf("exit\n");
		exit(0);
	}
	if (output != NULL && output[0] != '\0' && output[0] != '\n')
		add_history(output);
	return (output);
}

/**
 * @author Alex Kernot
 * @brief forks the current process and runs readline() in the child process,
 * setting up signals to stop readline() without disrupting the terminal.
 * @param prompt the prompt for readline() to display
 * @return the line entered into the terminal, with environment variables and
 * quotation marks already handled and ready for parsing.
*/
char	*wait_for_input(const char *prompt)
{
	int		pid;
	int		fds[2];
	char	*output;

	ft_pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &exit);
		signal(SIGSTOP, SIG_DFL);
		output = readline(prompt);
		output = preprocess(output);
		transmit(fds[1], output);
		close(fds[1]);
		exit(0);
	}
	output = get_input(pid, fds[0]);
	close(fds[0]);
	return (output);
}
