/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:34:03 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:47:09 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shunting_yard.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "tokenizer.h"
#include "stack.h"
#include "syntax_tree.h"
#include "libft.h"
#include "builtin.h"

/** 
 * @author Alex Kernot
 * STATIC:
 * checks if a command is a builtin by hashing the string then comparing it to
 * the predefined hash values for each builtin. It also checks the length of the
 * command just in case of hash collisions.
*/
t_bool	is_builtin(const char *str)
{
	if (ft_strncmp("echo", str, 4) == 0)
		return (true);
	if (ft_strncmp("pwd", str, 3) == 0)
		return (true);
	if (ft_strncmp("export", str, 6) == 0)
		return (true);
	if (ft_strncmp("cd", str, 2) == 0)
		return (true);
	if (ft_strncmp("unset", str, 5) == 0)
		return (true);
	if (ft_strncmp("env", str, 3) == 0)
		return (true);
	if (ft_strncmp("exit", str, 4) == 0)
		return (true);
	return (false);
}

int	is_operator(const char *token)
{
	if (token == NULL)
		return (0);
	if (token[0] == '(' && token[1] == '\0')
		return (1);
	if (token[0] == ')' && token[1] == '\0')
		return (1);
	if (token[0] == '|' && ((token[1] == '|' && token[2] == '\0')
			|| token[1] == '\0'))
		return (1);
	if (token[0] == '&' && token[1] == '&' && token[2] == '\0')
		return (1);
	return (0);
}

int	get_precedence(const char *token)
{
	if (token == NULL)
		return (0);
	if (token[0] == '(' && token[1] == '\0')
		return (10);
	if (token[0] == ')' && token[1] == '\0')
		return (10);
	if (token[0] == '|' && token[1] == '|' && token[2] == '\0')
		return (1);
	if (token[0] == '&' && token[1] == '&' && token[2] == '\0')
		return (1);
	if (token[0] == '|' && token[1] == '\0')
		return (2);
	return (0);
}

int	is_brackets(const char *token)
{
	if (token == NULL)
		return (0);
	if (token[0] == '(' && token[1] == '\0')
		return (1);
	if (token[0] == ')' && token[1] == '\0')
		return (1);
	return (0);
}

int	is_redirect(const char *token)
{
	if (token == NULL)
		return (0);
	if (token == NULL || token[0] == '\0')
		return (0);
	if (token[0] == '<' && token[1] == '\0')
		return (1);
	if (token[0] == '<' && token[1] == '<' && token[2] == '\0')
		return (1);
	if (token[0] == '>' && token[1] == '\0')
		return (1);
	if (token[0] == '>' && token[1] == '>' && token[2] == '\0')
		return (1);
	return (0);
}
