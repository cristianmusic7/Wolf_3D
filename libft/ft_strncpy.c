/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/16 12:46:04 by cfranco           #+#    #+#             */
/*   Updated: 2018/01/16 12:46:17 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int c;
	unsigned int c2;

	c = 0;
	c2 = 0;
	while (src[c2] != '\0')
		c2++;
	while (c < n)
	{
		if (c > c2)
			dest[c] = '\0';
		else
			dest[c] = src[c];
		c++;
	}
	return (dest);
}
