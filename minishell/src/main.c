/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 17:34:03 by akernot           #+#    #+#             */
/*   Updated: 2024/09/14 22:10:56 by akernot          ###   ########.fr       */
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

noreturn static void	stdin_err(const char *message)
{
	if (message != NULL)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, message, ft_strlen(message));
		write(STDOUT_FILENO, "\n", 1);
	}
	exit(1);
}

static int	read_stdin(char *input)
{
	const int	max_command_size = 4096;
	int		write_return;
	int		i;

	i = 0;
	while (i < max_command_size)
	{
		write_return = read(STDIN_FILENO, &input[i], 1);
		if (write_return == 0)
			return (1);
		if (write_return != 1)
		{
			input[i] = '\0';
			perror(NULL);
			stdin_err("error reading from file");
		}
		if (input[i] == '\n')
		{
			input[i] = '\0';
			return (0);
		}
		++i;
	}
	stdin_err("command longer than 4096 characters.");
	// char *str = get_next_line(STDIN_FILENO);
	// ft_memcpy(input, str, ft_strlen(str));
	// free(str);
}

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
	char	input[4097];
	int	last_return;
	int	read_status;

	last_return = 0;
	while (true)
	{
		ft_bzero(input, sizeof(input));
		read_status = read_stdin(input);
		if (read_status != 0)
			return (last_return);
		// write(STDOUT_FILENO, "\n", 1);
		// fflush(stdout);
		last_return = run(input);
	}
	return (last_return);
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
