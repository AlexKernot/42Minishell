/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:59:38 by akernot           #+#    #+#             */
/*   Updated: 2024/01/25 19:08:37 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_H
# define RUN_H

# include "segment.h"

void		run(char *input);
void		run_command(t_segment *segment);
int			create_subshells(t_segment *input, int size);
int			run_without_subshell(t_segment *segment);
int			process_exit_status(int retval);

#endif
