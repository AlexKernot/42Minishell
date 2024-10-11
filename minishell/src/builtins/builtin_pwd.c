/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:55:29 by pchawda           #+#    #+#             */
/*   Updated: 2024/09/12 18:16:16 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "builtin.h"
#include "environment_variables.h"

void	run_pwd(void)
{
	const char	*argv[3] = {"/bin/pwd", "-L", NULL};
	char		**envp;
	int			pid;

	pid = fork();
	if (pid == 0)
	{
		envp = compress_env_vars();
		execve("/bin/pwd", (char *const *)argv, envp);
		perror("minishell: pwd");
		exit(1);
	}
	waitpid(pid, NULL, 0);
	return ;
}

/**
 * @author Prachi Chawda
*/
int	builtin_pwd(int ac, char *av[])
{
	(void)ac;
	(void)av;
	run_pwd();
	return (0);
}
