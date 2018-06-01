/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/16 12:45:35 by cfranco           #+#    #+#             */
/*   Updated: 2018/01/16 12:45:38 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strstr(char *str, char *to_find)
{
	int c;
	int fc;
	int neq;

	c = 0;
	if (to_find[c] == '\0')
		return (str);
	while (str[c] != '\0')
	{
		fc = 0;
		if (str[c] == to_find[fc])
		{
			neq = 0;
			while (to_find[fc] != '\0')
			{
				if (to_find[fc] != str[c + fc])
					neq = 1;
				fc++;
			}
			if (!neq)
				return (str + c);
		}
		c++;
	}
	return ((void *)0);
}
