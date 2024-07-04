/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:35:33 by akernot           #+#    #+#             */
/*   Updated: 2023/10/26 14:26:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

char	*get_next_line(int fd);
int		gnl_linelen(const char *s);
void	*gnl_calloc(int nmemb, int size);
void	*gnl_memcpy(void *dest, const void *src, int n);
char	*gnl_strjoin(char const *s1, char const *s2);
int		check_newline(char *string);
#endif
