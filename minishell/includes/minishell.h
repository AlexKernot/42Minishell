/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 18:22:36 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 15:21:57 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "stack.h"

typedef enum type
{
	none,
	character,
	space,
	quotation,
	environment_var,
	symbol
}	t_char_type;

void	start_interactive(void);
char	*wait_for_input(const char *prompt);
char	*generate_prompt(const char *combined);
char	**vector_subset(t_vector *vec, int start, int end);
void	execute(const char *command, char *args[]);
int		hash_func(char const *string);
int		ft_arrlen(char **array);
int		handle_quotation(const t_vector *split,
			const t_vector *processed, int i);
char	*merge(const char *a, char *b, char *c);
void	clear_array(char **array);
char	**char_array_subset(char **input, int start, int end);
char	*preprocess(char *input);

#endif
