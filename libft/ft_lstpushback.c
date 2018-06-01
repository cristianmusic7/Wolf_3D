/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpushback.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 11:15:13 by cfranco           #+#    #+#             */
/*   Updated: 2018/03/26 11:15:15 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*ft_lstpushback(t_list **alst, void const *content,
							size_t content_size)
{
	t_list *new;
	t_list *current;

	current = *alst;
	new = ft_lstnew(content, content_size);
	if (*alst == NULL)
	{
		*alst = new;
		return (new);
	}
	while (current->next != NULL)
		current = current->next;
	current->next = new;
	return (new);
}
