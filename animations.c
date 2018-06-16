/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 15:05:10 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/16 15:05:17 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>

void	enemy_anim(t_view *view)
{
	int	c;

	c = 11;
	while (c < view->num_sprites)
	{
		if (view->sprites[c].texture == 14)
			view->sprites[c].texture = 11;
		else
			view->sprites[c].texture += 1;
		if (view->sprites[c].x > view->posX)
			view->sprites[c].x -= 0.1;
		else if (view->sprites[c].x < view->posX)
			view->sprites[c].x += 0.1;
		if (view->sprites[c].y > view->posY)
			view->sprites[c].y -= 0.1;
		else if (view->sprites[c].y < view->posY)
			view->sprites[c].y += 0.1;
		c++;
	}
}

void	fight_anim(t_view *view)
{
	if (view->fight_anim == 1)
	{
		if (view->fight_tex == 17)
		{
			view->fight_tex = 15;
			view->fight_anim = 0;
		}
		else
			view->fight_tex += 1;
	}
	else if (view->fight_anim == 2)
	{
		if (view->fight_tex == 20)
		{
			view->fight_tex = 15;
			view->fight_anim = 0;
		}
		else
			view->fight_tex += 1;
	}
}
