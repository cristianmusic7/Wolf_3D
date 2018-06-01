/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 14:45:37 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/22 14:45:39 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strmap(char const *s, char (*f)(char))
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
				new[c] = (*f)(*(s + c));
				c++;
			}
			new[c] = '\0';
		}
	}
	return (new);
}
