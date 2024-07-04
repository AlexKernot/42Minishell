/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_transmit.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 19:01:49 by akernot           #+#    #+#             */
/*   Updated: 2024/01/25 19:28:59 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRANSMIT_H
# define FT_TRANSMIT_H

void	transmit(int write_fd, char *message);
char	*receive(int read_fd);

#endif
