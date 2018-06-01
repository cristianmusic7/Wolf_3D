/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/23 10:38:00 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/23 10:38:04 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strtrim(char const *s)
{
	int		c;
	char	*end;
	char	*new;

	new = NULL;
	if (s)
	{
		c = ft_strlen(s) - 1;
		end = (char *)&(s[c]);
		while (c >= 0 && (*end == ' ' || *end == '\n' || *end == '\t'))
			end--;
		while (*s != '\0' && (*s == ' ' || *s == '\n' || *s == '\t'))
			s++;
		if (s > end)
			new = ft_strnew(0);
		else
		{
			new = ft_strnew(end++ - s + 1);
			if (new)
				while (--end >= s)
					new[end - s] = *end;
		}
	}
	return (new);
}
