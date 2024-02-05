/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 00:42:35 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:46:46 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
