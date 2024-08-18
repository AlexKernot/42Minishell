/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 17:01:18 by akernot           #+#    #+#             */
/*   Updated: 2024/08/15 22:32:51 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include <stddef.h>

char	*expand(const char *str, const int last_return);
char	*ft_strnjoin(const char *str1, const char *str2, int max_size);
char	*extract_and_join(const char *str, const char *result,
		const size_t size, const int last_return);
int	find_dollar_symbol(const char *str);
char 	*expand_str(const char *str, const char *result, int *i,
		const int last_return);
	

#endif
