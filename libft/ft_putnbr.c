/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 11:43:33 by cfranco           #+#    #+#             */
/*   Updated: 2018/01/15 17:22:11 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_putnbr(int nb)
{
	char	c;
	int		modh;
	int		sign;

	sign = 1;
	modh = 1;
	if (nb < 0)
	{
		ft_putchar('-');
		sign = -1;
	}
	while (modh <= (nb / 10) * sign)
		modh *= 10;
	while (modh >= 1)
	{
		c = ((nb / modh) % 10 * sign) + '0';
		ft_putchar(c);
		modh /= 10;
	}
}
