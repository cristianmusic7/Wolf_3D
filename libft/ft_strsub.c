/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 15:37:41 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/22 15:37:43 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	c;
	size_t	c2;
	char	*new;

	c = 0;
	c2 = 0;
	new = NULL;
	if (s)
	{
		new = (char *)malloc(sizeof(char) * (len + 1));
		if (new)
		{
			c = 0;
			while (s[c] != '\0' && c2 < len)
			{
				if (c++ >= start)
					new[c2++] = s[c - 1];
			}
			new[c2] = '\0';
		}
	}
	return (new);
}
