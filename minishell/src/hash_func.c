/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 00:42:35 by akernot           #+#    #+#             */
/*   Updated: 2024/07/21 16:57:05 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "minishell.h"

/**
 * @author Alex Kernot
 * @brief hashes a string and returns its number.
 * @param string the string to hash.
 * @return the hash of the string.
*/
int	hash_func(char const *string)
{
	const int	hash_const = 0x55555555;
	int			result;
	int			i;

	if (string == NULL)
		return (0);
	i = 0;
	result = hash_const;
	while (string[i])
	{
		result ^= string[i];
		result += ((result << 7) | (result >> 25));
		++i;
	}
	return (-result);
}
