/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:30:37 by akernot           #+#    #+#             */
/*   Updated: 2024/08/12 14:32:19 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "evaluator.h"

#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "ft_pipe.h"
#include "run.h"
#include "environment_variables.h"

static int create_subshell(t_command *cmd, int in_fd, int out_fd)
{
	pid_t 		pid;
	int 		retval;

	pid = fork();
	if (pid != 0)
	{
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		waitpid(pid, &retval, 0);
		return (process_exit_status(retval));
	}
	signal(SIGINT, SIG_DFL);
	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	run_path(cmd->args->array);
	perror("minishell: ");
	exit(1);
}

static int run_pipe(t_syntax_tree *tree, int in_fd, int out_fd)
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
	retval1 = evaluate_commands(tree->left, in_fd, fds[WRITE_FD]);
	close(fds[WRITE_FD]);
	if (retval1 != 0)
		return (retval1);
	retval2 = evaluate_commands(tree->right, fds[READ_FD], out_fd);
	close(fds[READ_FD]);
	return (retval2);
}

static int or(t_syntax_tree *tree, int in_fd, int out_fd)
{
	if (tree == NULL || tree->left == NULL || tree->right == NULL)
	{
		write(STDERR_FILENO, "parse error near `||`\n", 22);
		return (1);
	}
	evaluate_commands(tree->left, in_fd, out_fd);
	return (evaluate_commands(tree->right, in_fd, out_fd));
}

static int and(t_syntax_tree *tree, int in_fd, int out_fd)
{
	int retval1;
	int retval2;

	if (tree == NULL || tree->left == NULL || tree->right == NULL)
	{
		write(STDERR_FILENO, "parse error near `&&`\n", 22);
		return (1);
	}
	retval1 = evaluate_commands(tree->left, in_fd, out_fd);
	if (retval1 != 0)
		return (retval1);
	retval2 = evaluate_commands(tree->right, in_fd, out_fd);
	return (retval2);
}

static int single(t_syntax_tree *tree, int in_fd, int out_fd)
{
	if (tree == NULL || tree->left == NULL || tree->right != NULL)
	{
		write(STDERR_FILENO, "single command syntax error\n", 28);
		return (1);
	}
	return (evaluate_commands(tree->left, in_fd, out_fd));
}

/* retval is retval of run command */
int	evaluate_commands(t_syntax_tree *tree, int in_fd, int out_fd)
{
	char *operator;

	if (tree == NULL)
		return (1);
	if (tree->contents.type == command)
	{
		return (create_subshell
			(tree->contents.contents.command, in_fd, out_fd));
	}
	operator = tree->contents.contents.operator_word;
	if (operator[0] == '|' && operator[1] == '\0')
		return (run_pipe(tree, in_fd, out_fd));
	if (operator[0] == '|' && operator[1] == '|' && operator[2] == '\0')
		return (or(tree, in_fd, out_fd));
	if (operator[0] == '&' && operator[1] == '&' && operator[2] == '\0')
		return (and(tree, in_fd, out_fd));
	else if (operator[0] == '.' && operator[1] == '\0')
		return (single(tree, in_fd, out_fd));
	write(STDERR_FILENO, "Invalid syntax.\n", 16);
	return (1);
}
