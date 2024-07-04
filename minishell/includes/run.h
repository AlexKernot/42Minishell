/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:59:38 by akernot           #+#    #+#             */
/*   Updated: 2024/05/01 16:48:10 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_H
# define RUN_H

# include "segment.h"

int			run(char *input);
void		run_command(t_segment *segment);
int			create_subshells(t_segment *input, int size);
int			run_without_subshell(t_segment *segment);
int			process_exit_status(int retval);

#endif
