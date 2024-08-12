/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 16:53:14 by akernot           #+#    #+#             */
/*   Updated: 2024/08/11 18:54:51 by akernot          ###   ########.fr       */
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
