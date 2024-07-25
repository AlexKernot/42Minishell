/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:53:26 by akernot           #+#    #+#             */
/*   Updated: 2024/07/21 16:58:08 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEGMENT_H
# define SEGMENT_H

# include "libft.h"
# include "stack.h"

typedef struct s_segment
{
	char		**command;
	t_vector	*redirects;
}	t_segment;

typedef enum e_segment_type
{
	exec,
	redir_in,
	redir_out,
	redir_append,
	redir_delim,
	subshell
}	t_segment_type;

t_segment		*segment(char **input);
t_segment_type	get_type(char	*input);
int				segment_len(t_segment *input);

#endif
