/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 00:09:04 by akernot           #+#    #+#             */
/*   Updated: 2024/08/11 19:19:43 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVALUATOR_H
# define EVALUATOR_H

# include "syntax_tree.h"

int	evaluate_commands(t_syntax_tree *tree, int in_fd, int out_fd);

#endif
