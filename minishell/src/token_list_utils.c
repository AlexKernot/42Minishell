/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:50:40 by akernot           #+#    #+#             */
/*   Updated: 2024/10/15 16:50:51 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "token_list.h"

uint16_t	token_list_size(const t_token_list *token_list)
{
	if (token_list == NULL)
		return (0);
	return (token_list->size);
}

char	*get_token(const t_token_list *token_list, uint16_t index)
{
	if (token_list == NULL || token_list->array == NULL)
		return (NULL);
	if (index >= token_list->size)
		return (NULL);
	return (token_list->array[index]);
}
