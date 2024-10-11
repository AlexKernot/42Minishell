/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 18:58:07 by akernot           #+#    #+#             */
/*   Updated: 2024/09/25 16:33:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "ft_pipe.h"
# include "syntax_tree.h"

typedef struct s_redirect
{
	char			*file_name;
	t_redirect_type	type;
}	t_redirect;

int		redirect(t_command *command);

#endif
