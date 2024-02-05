/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:34:03 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:47:03 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

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
static void	start_stream(void)
{
	char	*input;

	input = get_next_line(STDIN_FILENO);
	run(input);
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
		rl_clear_history();
	}
	else
		start_stream();
	clear_env_vars();
	return (0);
}
