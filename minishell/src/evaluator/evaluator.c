/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:30:37 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 19:08:38 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluator.h"

#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "ft_pipe.h"
#include "run.h"
#include "environment_variables.h"
#include "builtin.h"
#include "redirect.h"

static int create_subshell(t_command *cmd, int in_fd, int out_fd,
				int last_return)
{
	pid_t 		pid;
	int 		retval;

	pid = fork();
	if (pid != 0)
	{
		waitpid(pid, &retval, 0);
		return (process_exit_status(retval));
	}
	signal(SIGINT, SIG_DFL);
	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	if (redirect(cmd, dup(STDIN_FILENO), last_return) == 1)
		exit(1);
	if (is_builtin(cmd->command))
		exit(run_builtin(cmd->args->array));
	run_path(cmd->args->array);
	perror("minishell: an unexpected error occurred");
	exit(1);
}

static int run_pipe(t_syntax_tree *tree, int in_fd, int out_fd, int last_return)
{
	int fds[2];
	int retval1;
	int retval2;

	if (tree == NULL || tree->left == NULL || tree->right == NULL)
	{
		write(STDERR_FILENO, "parse error near `|`\n", 21);
		return (1);
	}
	if (ft_pipe(fds) != 0)
	{
		write(STDERR_FILENO, "Could not create pipes\n", 23);
		return (1);
	}
	retval1 = evaluate_commands(tree->left, in_fd, fds[WRITE_FD],
					last_return);
	close(fds[WRITE_FD]);
	// if (retval1 != 0)
	//	return (retval1);
	retval2 = evaluate_commands(tree->right, fds[READ_FD], out_fd,
					last_return);
	close(fds[READ_FD]);
	return (retval2);
}

static int single(t_syntax_tree *tree, int in_fd, int out_fd, int last_return)
{
	if (tree == NULL || tree->left == NULL || tree->right != NULL)
	{
		write(STDERR_FILENO, "single command syntax error\n", 28);
		return (1);
	}
	return (evaluate_commands(tree->left, in_fd, out_fd, last_return));
}

/* retval is retval of run command */
int	evaluate_commands(t_syntax_tree *tree, int in_fd, int out_fd,
				int last_return)
{
	char *operator;

	if (tree == NULL)
		return (1);
	if (tree->contents.type == command)
	{
		return (create_subshell
			(tree->contents.contents.command, in_fd, out_fd,
					last_return));
	}
	operator = tree->contents.contents.operator_word;
	if (operator[0] == '|' && operator[1] == '\0')
		return (run_pipe(tree, in_fd, out_fd, last_return));
	else if (operator[0] == '.' && operator[1] == '\0')
		return (single(tree, in_fd, out_fd, last_return));
	write(STDERR_FILENO, "Invalid syntax.\n", 16);
	return (1);
}
