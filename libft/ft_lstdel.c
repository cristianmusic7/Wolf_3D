/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/25 18:43:28 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/27 23:40:55 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list *temp;

	if (alst)
	{
		while ((*alst)->next != NULL)
		{
			temp = *alst;
			ft_lstdelone(&(*alst), (*del));
			*alst = temp->next;
		}
		ft_lstdelone(&(*alst), (*del));
	}
}
