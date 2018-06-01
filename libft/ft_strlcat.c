/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/16 21:09:23 by cfranco           #+#    #+#             */
/*   Updated: 2018/01/16 21:09:25 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t c1;
	size_t c2;

	c1 = ft_strlen(dst);
	c2 = ft_strlen(src);
	if (!size || c1 >= size)
		return (size + c2);
	if (c2 <= (size - c1 - 1))
		ft_memcpy(dst + c1, src, c2 + 1);
	else
	{
		ft_memcpy(dst + c1, src, (size - c1 - 1));
		*(dst + size - 1) = '\0';
	}
	return (c1 + c2);
}
