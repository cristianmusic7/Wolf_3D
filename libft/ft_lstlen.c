/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/27 23:40:12 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/27 23:40:15 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int		ft_lstlen(t_list *begin_list)
{
	int c;

	c = 0;
	while (begin_list != NULL)
	{
		begin_list = begin_list->next;
		c++;
	}
	return (c);
}
