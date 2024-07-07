/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:50:47 by akernot           #+#    #+#             */
/*   Updated: 2024/07/07 16:15:32 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "token_list.h"

void		resize(t_token_list *token_list, uint16_t size)
{
	t_token	*new_array;
	int		i;

	new_array = (t_token *)malloc(sizeof(*new_array) * size);
	if (new_array == NULL)
		return ;
	i = 0;
	while (i < token_list->size)
	{
		new_array[i] = token_list->array[i];
		++i;
	}
	token_list->capacity = size;
	free(token_list->array);
	token_list->array = new_array;
}

void	push_token(t_token_list *token_list, const char *string)
{
	t_token		*array;
	uint16_t	size;
	
	if (token_list->size + 1 >= token_list->capacity)
		resize(token_list, token_list->capacity * 2);
	array = token_list->array;
	size = token_list->size;
	array[size].content = string;
	token_list->size += 1;
	return ;
}

void		delete_token_list(t_token_list **token_list)
{
	if (token_list == NULL || (*token_list) == NULL)
		return;
	if ((*token_list)->array != NULL)
	{
		free((*token_list)->array);
		(*token_list)->array = NULL;
	}
	free(*token_list);
	*token_list = NULL;
}

uint16_t	token_list_size(t_token_list *token_list)
{
	if (token_list == NULL)
		return (0);
	return (token_list->size);
}

const char	*get_token(t_token_list *token_list, uint16_t index)
{
	if (token_list == NULL || token_list->array == NULL)
		return (NULL);
	if (index >= token_list->size)
		return (NULL);
	return (token_list->array[index].content);
}
