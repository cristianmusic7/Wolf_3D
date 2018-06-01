/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 20:08:53 by cfranco           #+#    #+#             */
/*   Updated: 2018/02/21 20:08:54 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_putnbr_fd(int nb, int fd)
{
	char	c;
	int		modh;
	int		sign;

	sign = 1;
	modh = 1;
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		sign = -1;
	}
	while (modh <= (nb / 10) * sign)
		modh *= 10;
	while (modh >= 1)
	{
		c = ((nb / modh) % 10 * sign) + '0';
		ft_putchar_fd(c, fd);
		modh /= 10;
	}
}
