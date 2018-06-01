/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 16:04:08 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/22 16:04:10 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		c;
	int		c2;
	char	*new;

	c = 0;
	c2 = 0;
	new = NULL;
	if (s1 && s2)
	{
		c = ft_strlen(s1);
		c2 = ft_strlen(s2);
		new = (char *)malloc(sizeof(char) * (c + c2 + 1));
		if (new)
		{
			c2 = -1;
			while (s1[++c2] != '\0')
				new[c2] = s1[c2];
			c = -1;
			while (s2[++c] != '\0')
				new[c2 + c] = s2[c];
			new[c2 + c] = '\0';
		}
	}
	return (new);
}
