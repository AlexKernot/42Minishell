/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 14:40:22 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:05:04 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

static int	get_length(const char *s, char c, int j)
{
	int	i;

	i = 0;
	while (s[j] != c && s[j] != 0)
	{
		i++;
		j++;
	}
	return (i);
}

static int	get_words(const char *s, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 1;
	if (s[i] == c)
		while (s[i] == c)
			i++;
	while (s[i] != 0)
	{
		if (s[i] == c && i > 0)
			if (s[i - 1] != c && i != 0)
				words++;
		i++;
	}
	if (s[i - 1] == c)
		words--;
	return (words);
}

static int	copy_to_array(const char *string, char **array, char sep, int words)
{
	int	i;
	int	j;
	int	word_length;

	i = 0;
	j = 0;
	while (i < words && string[j] != 0)
	{
		while (string[j] == sep && string[j] != 0)
			j++;
		word_length = get_length(string, sep, j);
		array[i] = ft_substr(string, j, word_length);
		if (array[i] == NULL)
			return (0);
		j += word_length;
		i++;
	}
	array[i] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	int		words;
	char	**array;

	if (s == NULL)
		return (NULL);
	if (c == 0 && *s == 0)
	{
		array = (char **)malloc(sizeof(char **));
		array[0] = NULL;
		return (array);
	}
	words = get_words(s, c);
	array = (char **)malloc(sizeof(int *) * (words + 1));
	if (!array)
		return (NULL);
	if (!copy_to_array(s, array, c, words))
		return (NULL);
	return (array);
}
/*
int	main(void)
{
 	char *s = "split  ||this|for|me|||||!|";
 	int i = 0;
 	char **result = ft_split("xxxxxxxxhello!", 'x');

 	while (result[i] != NULL)
 	{
 		printf("%s\n", result[i]);
 		free(result[i]);
 		i++;
 	}
	printf("%s\n", result[i]);
 	free(result[i]);
 	free(result);
}*/