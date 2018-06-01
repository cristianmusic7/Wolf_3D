/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/16 14:47:03 by cfranco           #+#    #+#             */
/*   Updated: 2018/01/16 14:47:06 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t c;

	c = 0;
	while (s1[c] != '\0' && c < n)
	{
		if ((unsigned char)s1[c] != (unsigned char)s2[c])
			return ((unsigned char)s1[c] - (unsigned char)s2[c]);
		c++;
	}
	if (c == n)
		return ((unsigned char)s1[c - 1] - (unsigned char)s2[c - 1]);
	return ((unsigned char)s1[c] - (unsigned char)s2[c]);
}
