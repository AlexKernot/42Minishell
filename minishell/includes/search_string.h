/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_string.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:55:55 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 17:24:01 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEARCH_STRING_H
# define SEARCH_STRING_H

# include "stddef.h"

int	get_delimator(const char *str, const int start);
int	find_dollar_symbol(const char *str, size_t start);

#endif