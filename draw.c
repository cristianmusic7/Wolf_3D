/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/15 20:35:45 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/15 20:35:55 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdio.h>

int		draw_line(int x, int tx, int side, t_view *v)
{
	int y_start;
	int y_end;
	int d;

	if ((y_start = -(int)(v->s_height / v->z_buffer[x])
								/ 2 + v->s_height / 2) < 0)
		y_start = 0;
	if ((y_end = (int)(v->s_height / v->z_buffer[x])
								/ 2 + v->s_height / 2) >= v->s_height)
		y_end = v->s_height - 1;
	while (y_start < y_end)
	{
		d = y_start * 256 - v->s_height * 128 +
							(int)(v->s_height / v->z_buffer[x]) * 128;
		v->tex_y = ((d * v->texHeight) /
						(int)(v->s_height / v->z_buffer[x])) / 256;
		if (side == 2 || side == 3)
			fill_image(side, x, y_start, tx, v->tex_y, 1, v);
		else
			fill_image(side, x, y_start, tx, v->tex_y, 0, v);
		y_start++;
	}
	return (0);
}

int		draw_floor(int x, double floor_x, double floor_y, t_view *v)
{
	double	weight;
	double	floor_x_tmp;
	double	floor_y_tmp;
	int		y;

	if ((y = (int)(v->s_height / v->z_buffer[x]) /
						2 + v->s_height / 2) >= v->s_height)
		y = v->s_height - 1;
	if (y++ < 0)
		y = v->s_height;
	while (y < v->s_height)
	{
		weight = (v->s_height / (2.0 * y - v->s_height)) / v->z_buffer[x];
		floor_x_tmp = weight * floor_x + (1.0 - weight) * v->posX;
		floor_y_tmp = weight * floor_y + (1.0 - weight) * v->posY;
		v->tex_x = (int)(floor_x_tmp * v->texWidth) % v->texWidth;
		v->tex_y = (int)(floor_y_tmp * v->texHeight) % v->texHeight;
		fill_image(4, x, y, v->tex_x, v->tex_y, 1, v);
		fill_image(6, x, (v->s_height - y), v->tex_x, v->tex_y, 1, v);
		y++;
	}
	return (0);
}

void	draw_sprites2(t_view *v, int sprite_x, t_list *s_list, double tr_y)
{
	int sprite_h = ABS((int)(v->s_height / (tr_y)));
	int sprite_w = ABS((int)(v->s_width / (tr_y)));
	int y;
	int d;

	if ((v->drawStartY = -sprite_h / 2 + v->s_height / 2) < 0)
		v->drawStartY = 0;
	if ((v->drawEndY = sprite_h / 2 + v->s_height / 2) >= v->s_height)
		v->drawEndY = v->s_height - 1;
	if ((v->drawStartX = -sprite_w / 2 + sprite_x) < 0)
		v->drawStartX = 0;
	if ((v->drawEndX = sprite_w / 2 + sprite_x) >= v->s_width)
		v->drawEndX = v->s_width - 1;
	while (v->drawStartX < v->drawEndX)
	{
		v->tex_x = (int)(256 * (v->drawStartX++ -
							(-sprite_w / 2 + sprite_x))
								* v->texWidth / sprite_w) / 256;
		if ((v->drawStartX - 1) > 0 < v->s_width && tr_y < v->z_buffer[(v->drawStartX - 1)])
		{
			y = v->drawStartY;
			while (y < v->drawEndY)
			{
				d = (y++) * 256 - v->s_height * 128 + sprite_h * 128;
				v->tex_y = ((d * v->texHeight) / sprite_h) / 256;
				fill_image(v->sprites[((t_sprite *)s_list->content)->index].texture, v->drawStartX, y - 1, v->tex_x, v->tex_y, 2, v);
			}
		}
	}
}

int		draw_sprites(t_view *v, double inv_det)
{
	t_list	*s_list;
	double	sprite_x;
	double	sprite_y;
	int		c;
	double	ty;
	double	tx;

	s_list = NULL;
	c = 0;
	while (c < v->num_sprites)
	{
		v->sprites[c].distance = ((v->posX - v->sprites[c].x) * (v->posX - v->sprites[c].x)
									+ (v->posY - v->sprites[c].y) * (v->posY - v->sprites[c].y));
		ft_lstpushback(&(s_list), &(v->sprites[c++]), sizeof(t_sprite));
	}
	s_list = ft_lstsort(s_list, compare);
	while (s_list != NULL)
	{
		sprite_x = v->sprites[((t_sprite *)s_list->content)->index].x - v->posX;
		sprite_y = v->sprites[((t_sprite *)s_list->content)->index].y - v->posY;
		tx = inv_det * (v->dirY * sprite_x - v->dirX * sprite_y);
		if ((ty = inv_det * (-v->planeY * sprite_x + v->planeX * sprite_y)) > 0)
			draw_sprites2(v, (int)((v->s_width / 2) * (1 + tx / ty)), s_list, ty);
		s_list = s_list->next;
	}
	return (0);
}

int		draw_fight(t_view *v, int sprite_x, int sprite_w, int sprite_h)
{
	int	stripe;
	int	y;

	if ((v->drawStartY = v->s_height - sprite_h) < 0)
		v->drawStartY = 0;
	if ((v->drawEndY = v->s_height) >= v->s_height)
		v->drawEndY = v->s_height - 1;
	if ((stripe = -sprite_w / 2 + sprite_x) < 0)
		stripe = 0;
	if ((v->drawEndX = sprite_w / 2 + sprite_x) >= v->s_width)
		v->drawEndX = v->s_width - 1;
	while (stripe < v->drawEndX)
	{
		v->tex_x = (int)(256 * (stripe++ - (-sprite_w / 2 + sprite_x))
									* 250 / sprite_w) / 256;
		y = v->drawStartY;
		while (y < v->drawEndY)
		{
			v->tex_y = (int)(256 * (y++ - (v->s_height - sprite_h))
									* 150 / sprite_h) / 256;
			fill_image(v->fight_tex, stripe, y - 1, v->tex_x, v->tex_y, 2, v);
		}
	}
	return (0);
}
