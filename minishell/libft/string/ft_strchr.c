/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <a1885158@adelaide.edu.au>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:02:06 by akernot           #+#    #+#             */
/*   Updated: 2024/10/14 17:32:14 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = c % 256;
	while (s[i] != 0 || c == 0)
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
/*
int main(void)
{
	char *s = "tripouille";
	int	c = 't' + 256;
	char *result = ft_strchr(s,c);
	printf("   ----- ft_strchr: -----\n");
	printf("Char:   %c\n", c);
	printf("Word:   %x (%s)\n", &s, s);
	printf("Result: %x (%s)\n", &result, result);
	printf("Diff:   %lx\n\n", &s - &result);
	return (0);
}*/
