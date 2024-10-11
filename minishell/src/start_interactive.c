/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_interactive.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 16:59:10 by akernot           #+#    #+#             */
/*   Updated: 2024/09/17 16:37:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "run.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/** 
 * @author Alex Kernot
 * STATIC:
 * prints the minishell splash screen that displays when you start minishell
 * in interactive mode.
*/
static void	splash_screen(void)
{
	write(STDERR_FILENO, "\nWelcome to\n", 13);
	write(STDERR_FILENO, " __  __ _       _     _          _ _ \n", 39);
	write(STDERR_FILENO, "|  \\/  (_)     (_)   | |        | | |\n", 39);
	write(STDERR_FILENO, "| \\  / |_ _ __  _ ___| |__   ___| | |\n", 39);
	write(STDERR_FILENO, "| |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |\n", 39);
	write(STDERR_FILENO, "| |  | | | | | | \\__ \\ | | |  __/ | |\n", 39);
	write(STDERR_FILENO, "|_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|\n\n", 40);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * generates the first half of the prompt to pass to readline(). It takes the
 * USER and HOSTNAME environment variables 
*/
static char	*generate_name(const char *user, const char *hostname)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin("\001\x1b[1;35m\002", user);
	tmp2 = ft_strjoin(tmp1, "@");
	free(tmp1);
	if (hostname != NULL)
		tmp1 = ft_strjoin(tmp2, hostname);
	else
		tmp1 = ft_strjoin(tmp2, "Minishell");
	free(tmp2);
	tmp2 = ft_strjoin(tmp1, "\001\x1b[0m\002:\001\x1b[1;31m\002");
	free (tmp1);
	return (tmp2);
}

/** 
 * @author Alex Kernot
 * STATIC:
 * Checks if either the HOSTNAME or NAME environment variables are defined.
 * If they are, return their value. Otherwise return the string literal
 * 'minishell'
*/
static char	*get_hostname(void)
{
	char	*env;

	env = getenv("HOSTNAME");
	if (env == NULL)
		env = getenv("NAME");
	if (env == NULL)
		env = "minishell";
	return (env);
}

/**
 * @author Alex Kernot
 * @brief Starts the terminal in interactive mode. It attempts to build the
 * first half of the prompt early using the user's USER and HOSTNAME environment
 * variables. It then starts the main loop waiting for user input and running
 * it.
*/
void	start_interactive(void)
{
	const char	*user = getenv("USER");
	const char	*hostname = get_hostname();
	const char	*combined = generate_name(user, hostname);
	char		*input;
	char		*prompt;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGSTOP, &exit);
	splash_screen();
	while (1)
	{
		prompt = generate_prompt(combined);
		input = wait_for_input(prompt);
		free(prompt);
		if (input == NULL)
			continue ;
		run(input);
		free(input);
	}
	ft_putchar('\n');
	free((void *)combined);
	free(input);
	free(prompt);
}
