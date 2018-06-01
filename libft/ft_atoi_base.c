/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 09:33:07 by exam              #+#    #+#             */
/*   Updated: 2018/02/20 10:07:22 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

long long int	calc_num(const char *str, int base, int c)
{
	long long int	num;

	num = 0;
	while (str[c] != '\0')
	{
		if (str[c] >= '0' && str[c] <= ('0' + (base - 1)))
			num = (str[c++] - 48) + num * base;
		else if (base > 10 && ft_tolower(str[c]) >= 'a' &&
			ft_tolower(str[c]) <= ('a' + (base - 11)))
			num = (str[c++] - 'a' + 10) + num * base;
		else
			break ;
	}
	return (num);
}

long long int	ft_atoi_base(const char *str, int base)
{
	long long int	num;
	int				c;
	int				sign;

	num = 0;
	c = 0;
	sign = 1;
	if (base > 16 || base < 2)
		return (0);
	while (str[c] == ' ' || str[c] == '\t' || str[c] == '\n' ||
		str[c] == '\r' || str[c] == '\v' || str[c] == '\f')
		c++;
	if (((str[c] == '-' && base == 10) || str[c] == '+'))
		sign = 44 - str[c++];
	if (str[c] == '\0')
		return (0);
	num = calc_num(str, base, c);
	return ((int)(num * sign));
}
