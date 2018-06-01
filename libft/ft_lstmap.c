/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/25 19:51:36 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/25 19:51:38 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *nlist;
	t_list *copy;

	copy = NULL;
	if (lst)
	{
		nlist = (*f)(ft_lstnew(lst->content, lst->content_size));
		copy = nlist;
		while (lst->next != NULL)
		{
			nlist->next = (*f)(ft_lstnew(lst->next->content,
								lst->next->content_size));
			lst = lst->next;
			nlist = nlist->next;
		}
	}
	return (copy);
}
