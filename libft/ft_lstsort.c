/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/27 23:25:52 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/27 23:25:55 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*ft_lstsort(t_list *lst, int (*cmp)())
{
	void	*tmp_content;
	size_t	tmp_size;
	t_list	*tmp;

	tmp = lst;
	while (lst->next != NULL)
	{
		if (((*cmp)(lst->content, lst->next->content)) == 0)
		{
			tmp_content = lst->content;
			tmp_size = lst->content_size;
			lst->content = lst->next->content;
			lst->content_size = lst->next->content_size;
			lst->next->content = tmp_content;
			lst->next->content_size = tmp_size;
			lst = tmp;
		}
		else
			lst = lst->next;
	}
	lst = tmp;
	return (lst);
}
