/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 14:53:31 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/22 14:53:33 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		c;
	char	*new;

	c = 0;
	new = NULL;
	if (s && f)
	{
		while (s[c] != '\0')
			c++;
		new = (char *)malloc(sizeof(char) * (c + 1));
		if (new)
		{
			c = 0;
			while (s[c] != '\0')
			{
				new[c] = (*f)(c, *(s + c));
				c++;
			}
			new[c] = '\0';
		}
	}
	return (new);
}
