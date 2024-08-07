/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_transmit.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 19:01:49 by akernot           #+#    #+#             */
/*   Updated: 2024/08/01 18:06:58 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRANSMIT_H
# define FT_TRANSMIT_H

void	transmit(int write_fd, const char *message);
char	*receive(int read_fd);

#endif
