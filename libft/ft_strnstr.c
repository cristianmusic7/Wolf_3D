/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 20:42:23 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/21 20:42:25 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	c;
	int		fc;
	int		neq;

	c = 0;
	if (to_find[c] == '\0')
		return ((char *)str);
	while (str[c] != '\0' && c < len)
	{
		fc = 0;
		if (str[c] == to_find[fc])
		{
			neq = 0;
			while (to_find[fc] != '\0')
			{
				if ((to_find[fc] != str[c + fc]) || (c + fc) > len)
					neq = 1;
				fc++;
			}
			if (!neq)
				return ((char *)(str + c));
		}
		c++;
	}
	return (NULL);
}
