/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:18:02 by akernot           #+#    #+#             */
/*   Updated: 2024/07/11 16:23:31 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stdint.h>

# include "token_list.h"

/* The types of tokens in a minishell command. */
typedef enum e_char_classification
{
	none_c,
	letter_c,
	symbol_c
} t_char_class;

/* The allowed symbols in a minishell command. */
typedef enum e_symbols
{
	none_s,
	in_s,
	out_s,
	pipe_s,
	and_s,
	bracket_s
}	t_symbols;

/**
 * @author Alex Kernot 
 * @brief Reads the input string and splits it into individual tokens that can
 * be parsed in a shunting yard algorithm.
 */
t_token_list	*tokenize(const char *string);

/**
 * @author Alex Kernot
 * @brief Returns whether the passed in character is either an allowed character
 * for a command, or a symbol for modifying commands.
 * @param a the character to categorize
 */
t_char_class	get_char_type(const char a);

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

/**
 * @author Alex Kernot
 * @brief allocates a new string and copies characters from index start, until
 * index end. The allocated string is guaranteed to be large enough to contain
 * all the characters, plus a null terminator. If this guarnatee cannot be
 * upheld, this function returns a NULL pointer.
 * @param string the string to copy from
 * @param start the index to start copying from
 * @param end the index to stop copying at
 */
char			*extract_string(const char *string,
					const uint16_t start, const uint16_t end);

/**
 * @author Alex Kernot
 * @brief Splits part of a string containing words into smaller strings
 * containing a single minishell instruction, before adding the substring into
 * the token list.
 * @param tokens the token list to insert the new token into
 * @param string the string to read from
 * @param start the index to start reading from
 * @param end the index to stop reading from			
 */	
void			add_words(t_token_list *tokens, const char *string,
					const uint16_t start, const uint16_t end);
	
/**
 * @author Alex Kernot
 * @brief Splits part of a string containing symbols into smaller strings
 * containing a single minishell instruction, before adding the substring into
 * the token list.
 * @param tokens the token list to insert the new token into
 * @param string the string to read from
 * @param start the index to start reading from
 * @param end the index to stop reading from
 */
void			add_symbols(t_token_list *tokens, const char *string,
					const uint16_t start, const uint16_t end);
			
/**
 * @author Alex Kernot
 * @brief Iterates over the string, splitting it into smaller substrings to be
 * read by the add_symbols and add_words functions. 
 * @param token_list the list to add tokens into
 * @param string the string to read
 */
void			parse(t_token_list *token_list, const char *string);

/**
 * @author Alex Kernot
 * @brief Returns the type of symbol passed into the function. This can be used
 * for identifying whether the character is allowed within a command and used
 * for splitting the string into tokens
 * @param a the character to categorize
 */
t_symbols		get_symbols(const char a);

#endif