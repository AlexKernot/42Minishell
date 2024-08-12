/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:34:03 by akernot           #+#    #+#             */
/*   Updated: 2024/08/11 18:41:06 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

#include "libft.h"
#include "minishell.h"
#include "environment_variables.h"
#include "run.h"

/**
 * @author Alex Kernot 
 * STATIC:
 * when minishell is run in a subshell, or is not connected to an interactive
 * terminal, it will read a line of input from stdin without displaying the
 * splash screen or prompt. It will then run the single command passed in before
 * terminating minishell
*/
static int	start_stream(void)
{
	char	*input;
	int		last_return;

	last_return = 0;
	input = get_next_line(STDIN_FILENO);
	while (input != NULL)
	{
		last_return = run(input);
		free (input);
		input = get_next_line(STDIN_FILENO);
	}
	return last_return;
}

/**
 * @author Alex Kernot
 * @brief Initializes the shell's environment variable then starts in either
 * interactive or stream mode depending on if both stdin and stdout are
 * connected to an interactive terminal.
*/
int	main(int ac, char *const av[], char *const envp[])
{
	(void)ac;
	(void)av;
	init_env_vars(envp);
	if (isatty(STDIN_FILENO) == true && isatty(STDOUT_FILENO) == true)
	{
		start_interactive();
		clear_history();
	}
	else
		return (start_stream());
	clear_env_vars();
	return (0);
}
