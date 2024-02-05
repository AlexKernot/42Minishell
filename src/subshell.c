/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 16:53:14 by akernot           #+#    #+#             */
/*   Updated: 2024/02/01 21:33:26 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "redirect.h"
#include "segment.h"
#include "run.h"

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define WRITE_PIPE 1
#define READ_PIPE 0

#define OLD_STDOUT 15

/**
 * @author Alex Kernot
 * @brief calculates the exit status of the last subshell executed
 * @param retval is the raw value returned from a wait_pid() call
 * @return the exit status of the subshell
*/
int	process_exit_status(int retval)
{
	if (WIFEXITED(retval))
		return (WEXITSTATUS(retval));
	if (WIFSIGNALED(retval))
	{
		write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(retval));
	}
	if (WIFSTOPPED(retval))
		return (WSTOPSIG(retval));
	return (0);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * overwrites stdin and stdout with corrosponding ends of a pipe, creates a new
 * fork, and runs the command at input.
 * @warning this is very much a norminette prevention problem. I was unable to
 * design a system for this that fit comfortably into 25 line sections.
*/
static void	run_subshell(t_segment *input, int *pipe_fds,
	int *retval, t_bool last)
{
	int	pid;

	pipe_fds[2 + READ_PIPE] = pipe_fds[READ_PIPE];
	if (pipe_fds[WRITE_PIPE] != 0)
		close(pipe_fds[WRITE_PIPE]);
	ft_pipe(pipe_fds);
	pid = fork();
	if (pid != 0)
	{
		wait(retval);
		close(pipe_fds[2 + READ_PIPE]);
		return ;
	}
	dup2(pipe_fds[2 + READ_PIPE], STDIN_FILENO);
	dup2(pipe_fds[WRITE_PIPE], STDOUT_FILENO);
	if (last == true)
		dup2(OLD_STDOUT, STDOUT_FILENO);
	run_command(input);
	exit(1);
}

/**
 * @author Alex Kernot
 * @brief generates a subshell for each piped command and connects the
 * standard input and output together. This function will block untill
 * the final program finishes.
 * @param input the commands for the subshells to run
 * @param size the number of subshells to create
 * @return the return value of the last executed pipe
*/
int	create_subshells(t_segment *input, int size)
{
	int		pid;
	int		pipe_fds[4];
	int		i;
	int		retval;
	t_bool	last;

	retval = 0;
	pipe_fds[READ_PIPE] = STDIN_FILENO;
	pipe_fds[WRITE_PIPE] = 0;
	dup2(STDOUT_FILENO, OLD_STDOUT);
	pid = fork();
	if (pid != 0)
	{
		wait(&retval);
		return (process_exit_status(retval));
	}
	i = 0;
	while (i < size)
	{
		last = (i == size - 1);
		run_subshell(&input[i], pipe_fds, &retval, last);
		++i;
	}
	close(pipe_fds[WRITE_PIPE]);
	exit(process_exit_status(retval));
}

/**
 * @author Alex Kernot
 * @brief identifies the type of the segment by looking at the first character.
 * @param input the string to check.
 * @return an enum corrosponding to the segment type.
*/
t_segment_type	get_type(char	*input)
{
	const char	first = input[0];
	char		second;

	if (first == '\0')
		return (exec);
	if (first == '|')
		return (subshell);
	second = input[1];
	if (first == '<' && second == '<')
		return (redir_delim);
	if (first == '<')
		return (redir_in);
	if (first == '>' && second == '>')
		return (redir_append);
	if (first == '>')
		return (redir_out);
	return (exec);
}
