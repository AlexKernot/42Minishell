/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:18:02 by akernot           #+#    #+#             */
/*   Updated: 2024/07/05 16:53:33 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stdlib.h>

typedef enum e_char_classification
{
	none_c,
	letter_c,
	symbol_c
} t_char_class;

typedef enum e_symbols
{
	none_s,
	in_s,
	out_s,
	pipe_s,
	and_s,
	quote_s
}	t_symbols;

typedef struct s_token
{
	char	*content;	
}	t_token;

typedef struct s_token_list
{
	t_token		*array;
	uint16_t	length;
	uint16_t	capacity;
}	t_token_list;

t_token_list	*tokenize(const char *string);
t_char_class	get_char_type(const char a);
void			add_token(t_token_list *token_list, const char *string);
/**
 * @author Alex Kernot
 * @brief Copies 'size' characters from src, starting at index 'start'. This
 * function ensures the string is null terminated. Please ensure the size of
 * dest is enought to copy 'size' characters, plus a null terminator. Copying
 * will stop at a null terminator, even if the requested number of characters
 * haven't been copied.
 * @param src string to copy from
 * @param dest string to copy into
 * @param start index to start copying from
 * @param size max characters to copy
 */
void			copy_string(const char *src, char *dest,
					const uint16_t start, const uint16_t size);
char			*extract_string(const char *string,
					const uint16_t start, const uint16_t end);
void			add_words(t_token_list *tokens, const char *string,
					const uint16_t start, const uint16_t end);
void			add_symbols(t_token_list *tokens, const char *string,
					const uint16_t start, const uint16_t end);
void			parse(t_token_list *token_list, const char *string);

t_symbols		get_symbols(const char a);

#endif