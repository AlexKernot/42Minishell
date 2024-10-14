/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:59:38 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 19:05:50 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_H
# define RUN_H

# include "syntax_tree.h"

int	run(char *input);
void	run_command(t_command *command, int last_return);
int	run_without_subshell(t_command *segment, int last_return);
int	process_exit_status(int retval);
int	run_path(char **substr);

#endif
