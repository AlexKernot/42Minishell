/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 17:15:43 by akernot           #+#    #+#             */
/*   Updated: 2024/01/28 18:46:06 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirect.h"

#include <stdlib.h>

/**
 * @author Alex Kernot
 * @brief frees all the strings within a given array before freeing
 * the array itself.
 * @param array the array to free.
*/
void	clear_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		++i;
	}
	free (array);
}
