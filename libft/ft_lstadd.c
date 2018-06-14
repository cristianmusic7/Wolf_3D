/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/25 19:03:27 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/25 19:03:30 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_lstadd(t_list **alst, t_list *new)
{
	t_list *temp;
	temp = NULL;

	if (alst)
	{
		temp = *alst;
		*alst = new;
		new->next = temp;
	}
}
