/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:28:49 by akernot           #+#    #+#             */
/*   Updated: 2024/07/09 16:18:48 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_LIST_H
# define TOKEN_LIST_H

# include <stdint.h>

typedef struct s_token
{
	const char	*content;
}	t_token;

typedef struct s_token_list
{
	t_token		*array;
	uint16_t	size;
	uint16_t	capacity;
}	t_token_list;

t_token_list	*create_token_list(void);
void		push_token(t_token_list *token_list, const char *string);
const char	*get_token(t_token_list *token_list, uint16_t index);
void		delete_token_list(t_token_list **token_list);
void		resize(t_token_list *token_list, uint16_t size);
uint16_t	token_list_size(t_token_list *token_list);

#endif