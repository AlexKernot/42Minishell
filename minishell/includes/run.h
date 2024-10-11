/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:59:38 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 16:33:00 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_H
# define RUN_H

# include "syntax_tree.h"

int	run(char *input);
void	run_command(t_command *command);
int	run_without_subshell(t_command *segment);
int	process_exit_status(int retval);
int	run_path(char **substr);

#endif
