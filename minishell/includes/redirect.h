/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:58:07 by akernot           #+#    #+#             */
/*   Updated: 2024/08/03 15:17:01 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "segment.h"
# include "ft_pipe.h"

typedef struct s_redirect
{
	char			*file_name;
	t_segment_type	type;
}	t_redirect;

int		redirect(t_segment	*segment);

#endif
