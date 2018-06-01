/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/25 15:01:27 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/25 15:01:28 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_itoa(int value)
{
	int		res;
	char	*str;
	int		c;

	c = (value <= 0);
	res = value;
	while (res != 0)
	{
		res /= 10;
		c++;
	}
	str = (char*)malloc(sizeof(char) * (c + 1));
	if (str)
	{
		if (value <= 0)
			str[0] = 48 - (value < 0) * 3;
		str[c] = '\0';
		while (value != 0)
		{
			res = value % 10;
			str[c-- - 1] = res * (1 - 2 * (value < 0)) + 48;
			value /= 10;
		}
	}
	return (str);
}
