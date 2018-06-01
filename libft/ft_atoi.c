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

int	ft_atoi(const char *str)
{
	long long int	num;
	int				c;
	int				sign;

	num = 0;
	c = 0;
	sign = 1;
	while (str[c] == ' ' || str[c] == '\t' || str[c] == '\n' ||
		str[c] == '\r' || str[c] == '\v' || str[c] == '\f')
		c++;
	if (str[c] == '-' || str[c] == '+')
		sign = 44 - str[c++];
	if (str[c] == '\0')
		return (0);
	while (str[c] >= '0' && str[c] <= '9')
	{
		if (9223372036854775807 - num < (str[c] - 48))
			return ((-1 - sign) / 2);
		num = (str[c++] - 48) + num * 10;
	}
	return ((int)(num * sign));
}
