/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 15:16:01 by akernot           #+#    #+#             */
/*   Updated: 2024/08/04 19:20:38 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PIPE_H
# define FT_PIPE_H

# ifndef WRITE_FD
#  define WRITE_FD 1
# endif

# ifndef READ_FD
#  define READ_FD 0
# endif

char	*create_tmp(void);
int		ft_pipe(int fds[2]);

#endif
