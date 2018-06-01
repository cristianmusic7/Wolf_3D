/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 01:10:51 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/22 01:10:53 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*c_dst;
	unsigned char	*c_src;

	i = 0;
	c_dst = (unsigned char*)dst;
	c_src = (unsigned char*)src;
	if (dst - src == 0)
		return (dst);
	if (src > dst)
	{
		while (i++ < len)
			c_dst[i - 1] = c_src[i - 1];
	}
	else
	{
		i = len;
		while (i-- > 0)
			c_dst[i] = c_src[i];
	}
	return (c_dst);
}
