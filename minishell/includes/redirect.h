/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:58:07 by akernot           #+#    #+#             */
/*   Updated: 2024/02/01 20:22:11 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "segment.h"

typedef struct s_redirect
{
	char			*file_name;
	t_segment_type	type;
}	t_redirect;

int		redirect(t_segment	*segment);
char	*create_tmp(void);
int		ft_pipe(int fds[2]);

#endif
