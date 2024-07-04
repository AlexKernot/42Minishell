/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akernot <akernot@student.42Adel.org.au>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:19:36 by akernot           #+#    #+#             */
/*   Updated: 2023/06/15 18:04:23 by akernot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "libft.h"

t_list	*copy_to_new(t_list *lst, t_list *head, void *(*f)(void *))
{
	void	*content;
	t_list	*new_list;

	new_list = head;
	while (lst)
	{
		content = f(lst->content);
		lst = lst->next;
		new_list->next = ft_lstnew(content);
		if (new_list->next == NULL)
			return (NULL);
		new_list = new_list->next;
	}
	return (head);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list_head;

	if (lst == NULL || f == NULL)
		return (NULL);
	new_list_head = ft_lstnew(f(lst->content));
	lst = lst->next;
	new_list_head = copy_to_new(lst, new_list_head, f);
	if (new_list_head == NULL)
	{
		ft_lstclear(&new_list_head, del);
		return (NULL);
	}
	return (new_list_head);
}
/*
void	*new_list(void *content)
{
	content = (void *)ft_strdup("OK !");
	return (content);
}

void	delete(void *content)
{
	*((int *)content) = 1;
}

int	main(void)
{
	t_list *l = ft_lstnew(ft_strdup(" 1 2 3 "));
	t_list *ret;

	l->next = ft_lstnew(ft_strdup("ss"));
	l->next->next = ft_lstnew(ft_strdup("-_-"));
	ret = ft_lstmap(l, new_list, NULL);
	if (!ft_strncmp(ret->content, "OK !", 4) 
	&& !ft_strncmp(ret->next->content, "OK !", 4) 
	&& !ft_strncmp(ret->next->next->content, "OK !", 5) 
	&& !ft_strncmp(l->content, " 1 2 3 ", 8) 
	&& !ft_strncmp(l->next->content, "ss", 2) 
	&& !ft_strncmp(l->next->next->content, "-_-", 4))
		{
			printf("PASS\n");
			return (0);
		}
	printf("FAIL\n");
	return (0);
}*/