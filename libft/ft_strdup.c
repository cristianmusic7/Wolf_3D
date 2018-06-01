/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/18 00:23:12 by cfranco           #+#    #+#             */
/*   Updated: 2018/01/18 00:23:18 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(char *src)
{
	char	*copy;
	int		c;

	c = 0;
	while (src[c] != '\0')
		c++;
	copy = (char*)malloc(sizeof(char) * (c + 1));
	if (copy == NULL)
		return (NULL);
	c = 0;
	while (src[c] != '\0')
	{
		copy[c] = src[c];
		c++;
	}
	copy[c] = '\0';
	return (copy);
}
