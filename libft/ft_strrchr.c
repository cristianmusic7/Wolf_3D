/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 20:31:05 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/21 20:31:07 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(char *str, int c)
{
	char *pos;

	pos = 0;
	while (*str != '\0')
	{
		if (*str == c)
			pos = str;
		str++;
	}
	if (c == '\0')
		return (str);
	else if (pos)
		return (pos);
	return ((void *)0);
}
