/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 12:13:27 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/22 12:15:02 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	*ft_memalloc(size_t size)
{
	unsigned char	*mem;
	void			*copy;
	size_t			i;

	i = 0;
	copy = malloc(size);
	mem = copy;
	if (copy)
	{
		while (i++ < size)
			mem[i - 1] = 0;
		return (copy);
	}
	else
		return (NULL);
}
