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
#include <stdio.h>
#include <time.h>

void	enemy_attack(t_view *v, int c)
{
	if (ABS(v->sprites[c].x - v->posX) <= 1
			&& ABS(v->sprites[c].y - v->posY) <= 2)
	{
		v->sprites[c].texture = 16;
		v->attack = 1;
		if (v->health < 245)
			v->health += 1;
		else
			init_view(v);
	}
	else
	{
		if (v->sprites[c].texture == 14 || v->sprites[c].texture == 16)
			v->sprites[c].texture = 11;
		else
			v->sprites[c].texture += 1;
		if (v->sprites[c].x > v->posX)
			v->sprites[c].x -= 0.1;
		else if (v->sprites[c].x < v->posX)
			v->sprites[c].x += 0.1;
		if (v->sprites[c].y > v->posY)
			v->sprites[c].y -= 0.1;
		else if (v->sprites[c].y < v->posY)
			v->sprites[c].y += 0.1;
	}
}

void	enemy_death(t_view *v, int c)
{
	if (v->sprites[c].texture == 21)
	{
		v->sprites[c].x = 0;
		v->sprites[c].y = 0;
		while (v->map.values[(int)v->sprites[c].x][(int)v->sprites[c].y] != 0)
		{
			v->sprites[c].x = (float)rand() / (RAND_MAX) * v->map.width;
			v->sprites[c].y = (float)rand() / (RAND_MAX) * v->map.height;
		}
		v->sprites[c].texture = 11;
		v->sprites[c].alive = 1;
	}
	else
		v->sprites[c].texture += 1;
}

void	enemy_anim(t_view *v)
{
	int	c;

	c = 11;
	v->attack = 0;
	while (c < v->num_sprites)
	{
		srand((unsigned int)time(NULL));
		if (v->sprites[c].alive)
			enemy_attack(v, c);
		else
			enemy_death(v, c);
		c++;
	}
}

void	kill_anim(t_view *v)
{
	int	c;

	c = 11;
	while (c < v->num_sprites)
	{
		if (ABS(v->sprites[c].x - v->posX) <= 1
				&& ABS(v->sprites[c].y - v->posY) <= 2)
		{
			v->sprites[c].texture = 17;
			v->sprites[c].alive = 0;
		}
		c++;
	}
}

void	fight_anim(t_view *view)
{
	if (view->fight_anim == 1)
	{
		if (view->fight_tex == 23)
		{
			view->fight_tex = 21;
			view->fight_anim = 0;
			if (view->attack)
				kill_anim(view);
		}
		else
			view->fight_tex += 1;
	}
	else if (view->fight_anim == 2)
	{
		if (view->fight_tex == 26)
		{
			view->fight_tex = 21;
			view->fight_anim = 0;
			if (view->attack)
				kill_anim(view);
		}
		else
			view->fight_tex += 1;
	}
}
